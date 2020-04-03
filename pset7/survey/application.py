import cs50
import csv

from flask import Flask, jsonify, redirect, render_template, request, url_for

# Configure application
app = Flask(__name__)

# Reload templates when they are changed
app.config["TEMPLATES_AUTO_RELOAD"] = True


@app.after_request
def after_request(response):
    """Disable caching"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET"])
def get_index():
    return redirect("/form")


@app.route("/form", methods=["GET"])
def get_form():
    return render_template("form.html")


@app.route("/form", methods=["POST"])
def post_form():
    # Handler the /form and the POST
    # And save data in database

    # Get the values
    email = request.form.get("email")
    suscribe_newsletter = request.form.get("suscribe_newsletter")
    sport = request.form.get("sport")
    genre = request.form.get("genre")

    # In case of errors present
    errors = []
    # Check values
    if not email:
        errors.append("Provied an email")
    if not sport:
        errors.append("Provied a sport")
    if not genre:
        errors.append("Provied genre")

    if len(errors) > 0:
        return render_template("error.html", message=errors)
    else:
        # Save to data base only is are valid values
        with open("survey.csv", "a") as file:
            writer = csv.DictWriter(file, fieldnames=["email", "suscribe_newsletter", "sport", "genre"])
            writer.writerow({
                "email": email,
                "suscribe_newsletter": suscribe_newsletter,
                "sport": sport,
                "genre": genre
            })
        return redirect(url_for("get_sheet"))


@app.route("/sheet", methods=["GET"])
def get_sheet():
    # Handler for /sheet.
    # It's show all the registers
    with open("survey.csv", "r") as file:
        reader = csv.DictReader(file, fieldnames=["email", "suscribe_newsletter", "sport", "genre"])
        registers = list(reader)

    return render_template("registers.html", registers=registers)