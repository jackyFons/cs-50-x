import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd, valid_username, valid_password, isInt, get_num_shares

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


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
    purchases = db.execute("SELECT * FROM purchases WHERE id = ?", session["user_id"])
    stocks = get_num_shares(purchases)
    index_shares = []

    for stock in stocks.keys():
        quote = lookup(stock)
        purchase_info = {}
        purchase_info["symbol"] = stock
        purchase_info["name"] = quote["name"]
        purchase_info["shares"] = stocks[stock]
        purchase_info["current_price"] = quote["price"]
        purchase_info["total"] = purchase_info["current_price"] * purchase_info["shares"]
        index_shares.append(purchase_info)

    # Calculate total user's cash balance and grand total
    get_cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]
    balance = get_cash["cash"]
    total = 0.0
    for t in index_shares:
        total += t["total"]
    total += balance
    return render_template("index.html", purchases=index_shares, balance=balance, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        # Ensure there is input
        if not request.form.get("symbol"):
            return apology("must provide a symbol", 400)
        elif not request.form.get("shares"):
            return apology("must provide number of shares", 400)
        # Ensure number is an int
        elif not isInt(request.form.get("shares")):
            return apology("shares must be a whole number", 400)
        # Ensure number of shares is positive
        elif int(request.form.get("shares")) < 1:
            return apology("shares should be at least 1", 400)

        # Ensure stock exists
        sym = request.form.get("symbol")
        if lookup(sym):
            quote = lookup(sym)
            num_shares = int(request.form.get("shares"))
            price_per_share = quote["price"]
            total_price = price_per_share * num_shares

            # Ensure user has enough cash
            user_cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]
            if user_cash["cash"] < total_price:
                return apology("not enough cash", 400)

            # Make transaction
            symbol = quote["symbol"]
            name = quote["name"]
            id = session["user_id"]
            date_time = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
            db.execute("INSERT INTO purchases (id, symbol, num_shares, price_per_share, total_price, date_time, type, name) VALUES (?, ?, ?, ?, ?, ?, ?, ?)",
                       id, symbol, num_shares, price_per_share, total_price, date_time, "buy", name)

            # Update user's cash
            db.execute("UPDATE users SET cash = ? WHERE id = ?", user_cash["cash"] - total_price, id)
            return redirect("/")
        # Stock symbol wasn't found
        else:
            return apology("stock symbol not found", 400)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # Retrieve user's purchases
    purchases = db.execute("SELECT * FROM purchases WHERE id = ?", session["user_id"])
    return render_template("history.html", purchases=purchases)


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
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
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


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        # Ensure there is input
        if not request.form.get("symbol"):
            return apology("there must be input", 400)

        # Send input to quoted.html
        symbol = request.form.get("symbol")
        if lookup(symbol):
            quote = lookup(symbol)
            return render_template("quoted.html", quote=quote)
        else:
            return apology("stock symbol not found", 400)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # Forgets any user-id
    session.clear()

    if request.method == "POST":
        # Ensure all information has been provided
        if not request.form.get("username"):
            return apology("must provide a username", 400)
        elif not request.form.get("password"):
            return apology("must provide a password", 400)
        elif not request.form.get("confirmation"):
            return apology("must provide confimation to password", 400)
        # Ensure confirmation is the same as password
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("password and confirmation must be the same", 400)
         # Ensure username does not contain ' or ;
        elif not valid_username(request.form.get("username")):
            return apology("username must not contain ' or ;", 400)
        # Ensure password contains a letter, number, and special character
        elif not valid_password(request.form.get("password")):
            return apology("password must contain a letter, number, and special character", 400)
        # Ensure username is not taken
        elif len(db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))) == 1:
            return apology("username is taken", 400)
        # Register new user
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)",
                   request.form.get("username"), generate_password_hash(request.form.get("password")))

        return redirect("/")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        # Ensure inputs all information is provided
        if not request.form.get("symbol"):
            return apology("must provide a symbol", 400)
        elif not request.form.get("shares"):
            return apology("must provide number of shares", 400)
        # Ensure shares is an int
        elif not isInt(request.form.get("shares")):
            return apology("shares must be a whole number", 400)
        # Ensure shares is greater than 0
        elif int(request.form.get("shares")) < 0:
            return apology("shares must be greater than 0", 400)

        # Get user's purchases
        purchases = db.execute("Select * FROM purchases WHERE id = ?", session["user_id"])

        # Return error if user has not made purchases
        if len(purchases) < 1:
            return apology("you have not bought any shares", 400)

        # Get number of stock shares user has
        stocks = get_num_shares(purchases)

        # Ensure user has enough shares to sell
        if stocks[request.form.get("symbol")] < int(request.form.get("shares")):
            return apology("you don't have enough shares to sell", 400)
        else:
            # Make transaction
            quote = lookup(request.form.get("symbol"))
            symbol = quote["symbol"]
            num_shares = int(request.form.get("shares"))
            price_per_share = quote["price"]
            name = quote["name"]
            total_price = num_shares * price_per_share
            date_time = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
            db.execute("INSERT INTO purchases (id, symbol, num_shares, price_per_share, total_price, date_time, type, name) VALUES (?, ?, ?, ?, ?, ?, ?, ?)",
                       session["user_id"], symbol, num_shares, price_per_share, total_price, date_time, "sell", name)

            # Update case
            user_cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]
            db.execute("UPDATE users SET cash = ? WHERE id = ?", user_cash["cash"] + total_price, session["user_id"])

        return redirect("/")
    else:
        purchases = db.execute("SELECT * FROM purchases WHERE id = ?", session["user_id"])
        stocks = get_num_shares(purchases)
        return render_template("sell.html", symbols=stocks.keys())


@app.route("/add_cash", methods=["GET", "POST"])
@login_required
def add_cash():
    if request.method == "POST":
        # Ensure there is input
        if not request.form.get("cash"):
            return apology("must provide an amount", 400)
        # Ensure the amount is greater than 0
        elif int(request.form.get("cash")) < 0:
            return apology("cash must be greater than 0", 400)

        # Update user's cash
        add_cash = int(request.form.get("cash"))
        current_cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
        db.execute("UPDATE users SET cash = ? WHERE id = ?", add_cash + current_cash, session["user_id"])
        return redirect("/")
    else:
        return render_template("add_cash.html")