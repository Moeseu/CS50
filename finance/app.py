import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    sum = 0
    user_info = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
    holdings = db.execute(
        "SELECT * FROM holdings WHERE user_id = ? ORDER BY holdings.symbol", session["user_id"])
    for holding in holdings:
        stock_info = lookup(holding["symbol"])
        holding["price"] = stock_info["price"]
        holding["total"] = holding["shares"] * holding["price"]
        sum += holding["total"]

    return render_template("index.html", user_info=user_info[0], holdings=holdings, sum=sum)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    user_info = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])[0]
    stock = 0
    """Buy shares of stock"""
    if request.method == "POST":
        symbol_input = request.form.get("symbol")
        symbol = lookup(symbol_input)
        shares = int(request.form.get("shares"))

        if not symbol_input or not shares:
            return render_template("buy.html", error="All fields are required.")
        if shares < 1:
            return render_template("buy.html", error="Shares must be more than 0.")
        if not symbol:
            return render_template("buy.html", error="Symbol doesn't exist.")

        price = symbol["price"]
        balance = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]

        stock = price * float(shares)
        new_balance = balance - stock

        if "update" in request.form:
            return render_template("buy.html", user_info=user_info, stock=stock)

        if new_balance < 0:
            db.execute("""
                INSERT INTO transactions (user_id, symbol, shares, price, previous_balance, new_balance, is_successful)
                VALUES (?, ?, ?, ?, ?, ?, ?)""",
                       session["user_id"], symbol["symbol"], shares, price, balance, new_balance, 0)
            return apology("Not enough funds")

        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_balance, session["user_id"])

        db.execute("""
            INSERT INTO transactions (user_id, symbol, shares, price, previous_balance, new_balance, is_successful)
            VALUES (?, ?, ?, ?, ?, ?, ?)""",
                   session["user_id"], symbol["symbol"], shares, price, balance, new_balance, 1)

        existing = db.execute("SELECT shares FROM holdings WHERE user_id = ? AND symbol = ?",
                              session["user_id"], symbol["symbol"])

        if existing:
            new_shares = existing[0]["shares"] + shares
            db.execute("UPDATE holdings SET shares = ? WHERE user_id = ? AND symbol = ?",
                       new_shares, session["user_id"], symbol["symbol"])
        else:
            db.execute("INSERT INTO holdings (user_id, symbol, shares) VALUES (?, ?, ?)",
                       session["user_id"], symbol["symbol"], shares)

        return redirect("/")
    return render_template("buy.html", user_info=user_info, stock=stock)


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    transactions = db.execute(
        "SELECT * FROM transactions WHERE user_id = ? ORDER BY transaction_date", session["user_id"])
    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")

# I'm not sure I understood the task completely :) I will try to do my best


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        symbol = request.form.get("symbol")

        if not symbol:
            return render_template("quote.html", error="Must provide symbol")

        quote = lookup(symbol)
        if quote is None:
            return render_template("quote.html", error="Invalid symbol")

        return render_template("quote.html", quote=quote)

    return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        password_repeat = request.form.get("password_repeat")

        if not username or not password or not password_repeat:
            return render_template("register.html", error="All fields are required.")

        if password != password_repeat:
            return render_template("register.html", error="Passwords do not match.")

        existing_user = db.execute("SELECT * FROM users WHERE username = ?", username)

        if existing_user:
            return render_template("register.html", error="Username already exists.")

        password_hash = generate_password_hash(password)
        db.execute("INSERT INTO users (username, hash, cash) VALUES (?, ?, ?)",
                   username, password_hash, 10000.0)

        return redirect("/")

    return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    user_info = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])[0]
    holdings = db.execute(
        "SELECT * FROM holdings WHERE user_id = ? ORDER BY symbol", session["user_id"])

    if not holdings:
        return render_template("sell.html", user_info=user_info, holdings=holdings, error="You have no stocks to sell")

    if request.method == "POST":
        symbol = request.form.get("stock")
        shares_to_sell = request.form.get("shares")
        action = request.form.get("action")

        if not symbol or not shares_to_sell:
            return render_template("sell.html", user_info=user_info, holdings=holdings, error="All fields are required.")

        try:
            shares_to_sell = int(shares_to_sell)
            if shares_to_sell <= 0:
                return render_template("sell.html", user_info=user_info, holdings=holdings, error="Shares must be greater than 0")
        except ValueError:
            return render_template("sell.html", user_info=user_info, holdings=holdings, error="Invalid shares number")

        holding = db.execute(
            "SELECT shares FROM holdings WHERE user_id = ? AND symbol = ?", session["user_id"], symbol)

        if not holding:
            return render_template("sell.html", user_info=user_info, holdings=holdings, error="You don't own this stock")

        if shares_to_sell > holding[0]["shares"]:
            return render_template("sell.html", user_info=user_info, holdings=holdings, error="Not enough shares")

        stock_info = lookup(symbol)
        if not stock_info:
            return render_template("sell.html", user_info=user_info, holdings=holdings, error="Failed to get stock price")

        stock_price = stock_info["price"]
        stock = stock_price * shares_to_sell

        if action == "update":
            return render_template("sell.html", user_info=user_info, holdings=holdings, stock=f"Estimated: ${stock:.2f}")

        new_balance = user_info["cash"] + stock

        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_balance, session["user_id"])

        remaining_shares = holding[0]["shares"] - shares_to_sell
        if remaining_shares > 0:
            db.execute("UPDATE holdings SET shares = ? WHERE user_id = ? AND symbol = ?",
                       remaining_shares, session["user_id"], symbol)
        else:
            db.execute("DELETE FROM holdings WHERE user_id = ? AND symbol = ?",
                       session["user_id"], symbol)

        db.execute("""
            INSERT INTO transactions (user_id, symbol, shares, price, previous_balance, new_balance, is_successful)
            VALUES (?, ?, ?, ?, ?, ?, ?)
            """,
                   session["user_id"], symbol, -
                   shares_to_sell, stock_price, user_info["cash"], new_balance, 1
                   )

        return redirect("/")

    return render_template("sell.html", user_info=user_info, holdings=holdings)
