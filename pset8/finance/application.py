import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    stocks = db.execute(
        "SELECT SUM(operations.shares) AS shares, symbols.symbol FROM operations INNER JOIN symbols ON symbols.id=operations.id_symbol WHERE id_user=:user_id GROUP BY operations.id_symbol;", user_id=session["user_id"])
    for stock in stocks:
        stock["price"] = lookup(stock["symbol"])["price"]
        stock["total_value"] = stock["price"] * stock["shares"]
    cash = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])[0]["cash"]

    total = cash
    for stock in stocks:
        total += stock["total_value"]
    return render_template("index.html", stocks=stocks, cash=cash, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # Check that symbol is valid
        quote = lookup(symbol)
        if not quote:
            return apology("invalid symbol", 400)
        # Ensure a valid amount of shares
        if not shares.isdigit():
            return apology("you need to int provide a positive value", 400)
        shares = int(shares)
        if shares <= 0:
            return apology("you need to int provide a positive value", 400)
        user = db.execute("SELECT * FROM users WHERE users.id = :user_id", user_id=session['user_id'])[0]
        if not user:
            return apology("error getting user", 400)

        # Check that user have enough money
        if quote["price"] * shares > float(user["cash"]):
            return apology("you don't have enough cash to buy")

        if len(db.execute("SELECT * from symbols WHERE symbol = :symbol", symbol=symbol)) == 0:
            db.execute("INSERT INTO symbols (symbol) VALUES (:symbol)", symbol=symbol)

        id_symbol = db.execute("SELECT * from symbols WHERE symbol = :symbol", symbol=symbol)[0]["id"]
        db.execute("INSERT INTO operations (id_user, id_symbol, shares, price ) VALUES (:user_id, :id_symbol, :shares, :price);",
                   user_id=session['user_id'], id_symbol=id_symbol, shares=shares, price=quote["price"])
        db.execute("UPDATE users SET cash = :cash", cash=user["cash"] - quote["price"]*shares)

        flash("Bought!")
        return redirect("/")

    return render_template("buy.html")


@app.route("/check", methods=["GET"])
def check():
    """Return true if username available, else false, in JSON format"""
    username = request.args.get("username")

    # Check that a username is provived
    if not username:
        return jsonify(False)

    # Look for users with that username
    users = db.execute("SELECT * FROM users WHERE username=:username", username=username)

    # Check if exist a user with that username
    if len(users):
        return jsonify(False)

    # The username is valid to use
    return jsonify(True)


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    operations = db.execute("SELECT * FROM operations INNER JOIN symbols ON operations.id_symbol=symbols.id WHERE id_user=:user_id;",
                            user_id=session["user_id"])

    return render_template("history.html", operations=operations)


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
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

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
        symbol = request.form.get("symbol")
        # Ensure that the symbol is provided
        if not symbol:
            return apology("must provide a symbol", 400)
        # Get the quote
        quote = lookup(symbol)
        # Check if is a valid quote
        if not quote:
            return apology("error getting quote", 400)

        return render_template("quoted.html", quote=quote)

    return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Ensure that username was submitted
        if not username:
            return apology("must provide username", 400)

        # Ensure that username was submitted
        if not password:
            return apology("must provide password", 400)

        # Ensure that username was submitted
        if not confirmation:
            return apology("must provide confirmation password", 400)

        # Query database for username to check that not exist
        user = db.execute("SELECT * FROM users WHERE username = :username;",
                          username=username)

        # Check username available
        if user:
            return apology("username already taken", 400)

        # Check that password and confirmation are equal
        if confirmation != password:
            return apology("confirmation and password don't match", 400)

        db.execute("INSERT INTO users (username, hash) VALUES (:username, :password);",
                   username=username, password=generate_password_hash(password))

        return redirect("/")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    stocks = db.execute("SELECT symbol FROM operations INNER JOIN symbols WHERE operations.id_symbol = symbols.id GROUP BY symbol;")

    if request.method == "POST":
        shares = int(request.form.get("shares"))
        stock = request.form.get("symbol")

        # Check a positive value for amount of shares
        if shares <= 0:
            return apology("you have to provide a postive value", 400)

        # Get info of the user for the given stock
        user_stock = db.execute(
            "SELECT symbol, id_symbol, SUM(shares) as shares FROM operations INNER JOIN symbols ON operations.id_symbol = symbols.id WHERE symbol = :symbol GROUP BY symbol;", symbol=stock)

        # Chack that user own that stock
        if not len(user_stock):
            return apology("You don't own the selected share", 400)

        # Check that user want to sell a valid ammount of shares
        if shares > user_stock[0]["shares"]:
            return apology("You don't have that amount of shares", 400)

        # Update database with the operation
        price = lookup(stock)["price"]
        db.execute(
            "INSERT INTO operations (id_user, id_symbol, shares, price) VALUES(:user_id, :symbol_id, :shares, :price)", user_id=session["user_id"], symbol_id=user_stock[0]["id_symbol"], shares=shares*-1, price=price)

        user = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])[0]
        db.execute("UPDATE users set cash = :cash WHERE id = :user_id",
                   cash=user["cash"] + shares*price, user_id=session["user_id"])

        # After complete operation print message and redirect
        flash("Soldt!")
        return redirect("/")
    # If get
    return render_template("sell.html", stocks=stocks)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
