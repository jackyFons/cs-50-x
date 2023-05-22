import os
import requests
import urllib.parse

from flask import redirect, render_template, request, session
from functools import wraps

# Symbols sql cant use
symbols = "';"

# Special characters
sc = "[@_!#$%^&*()<>?/\|}{~:]"


def apology(message, code=400):
    """Render message as an apology to user."""
    def escape(s):
        """
        Escape special characters.

        https://github.com/jacebrowning/memegen#special-characters
        """
        for old, new in [("-", "--"), (" ", "-"), ("_", "__"), ("?", "~q"),
                         ("%", "~p"), ("#", "~h"), ("/", "~s"), ("\"", "''")]:
            s = s.replace(old, new)
        return s
    return render_template("apology.html", top=code, bottom=escape(message)), code


def login_required(f):
    """
    Decorate routes to require login.

    https://flask.palletsprojects.com/en/1.1.x/patterns/viewdecorators/
    """
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if session.get("user_id") is None:
            return redirect("/login")
        return f(*args, **kwargs)
    return decorated_function


def lookup(symbol):
    """Look up quote for symbol."""

    # Contact API
    try:
        api_key = os.environ.get("API_KEY")
        url = f"https://cloud.iexapis.com/stable/stock/{urllib.parse.quote_plus(symbol)}/quote?token={api_key}"
        response = requests.get(url)
        response.raise_for_status()
    except requests.RequestException:
        return None

    # Parse response
    try:
        quote = response.json()
        return {
            "name": quote["companyName"],
            "price": float(quote["latestPrice"]),
            "symbol": quote["symbol"]
        }
    except (KeyError, TypeError, ValueError):
        return None


def usd(value):
    """Format value as USD."""
    return f"${value:,.2f}"


def valid_username(username):

    # Ensure username does not contain , or ;
    if any(character in symbols for character in username):
        return False
    return True


def valid_password(password):

    # Ensure password has a letter, number, and special character
    if not any(character in sc for character in password):
        return False
    if not any(character.isalpha() for character in password):
        return False
    if not any(character.isdigit() for character in password):
        return False
    return True


def isInt(num):
    try:
        int(num)
        return True
    except ValueError:
        return False


def get_num_shares(purchases):

    # Calculate remaining shares for each stock purchased
    stock_shares = {}
    for purchase in purchases:
        if purchase["symbol"] in stock_shares:
            if purchase["type"] == "buy":
                stock_shares[purchase["symbol"]] += purchase["num_shares"]
            elif purchase["type"] == "sell":
                stock_shares[purchase["symbol"]] -= purchase["num_shares"]
        else:
            stock_shares[purchase["symbol"]] = purchase["num_shares"]

    # Finds stocks with 0 shares (user has sold them)
    has_zero = []
    for stock in stock_shares.keys():
        if stock_shares[stock] == 0:
            has_zero.append(stock)

    # Removes stocks with no shares
    for stock in has_zero:
        del stock_shares[stock]
    return stock_shares