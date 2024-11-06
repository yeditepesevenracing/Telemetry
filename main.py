import threading
from flask import Flask, request, render_template, redirect, url_for
from flask_sqlalchemy import SQLAlchemy
from dbModels import TestData, db
import requests

from observingValues import time_data

app = Flask(__name__)

app.config["SECRET_KEY"] = "gizli-anahtar"
app.config["SQLALCHEMY_DATABASE_URI"] = "sqlite:///database.db"
db.init_app(app)

# anlık veri izleme ve anlık veri kaydetme olarak iki farklı fonsksiyon oluşturulailir


# index sayfası telemetry nin ana sayfası olacak
@app.route("/")
def index():
    return render_template("index.html")

@app.route("/api/data", methods=["POST"])
def api_data():
    data = request.json
    new_data = TestData(
        time=data["time"],
        data=data["data"]
    )
    db.session.add(new_data)
    db.session.commit()
    return "Data added to database"

if __name__ == "__main__":
    db.create_all()
    thread = threading.Thread(target=api_data)
    thread.daemon = True
    thread.start()
    app.run(debug=True)




