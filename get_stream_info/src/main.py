from flask import Flask, request
import yt_dlp
import json
import datetime
import sys
from pytube import extract
import psycopg2 as ps
from flask_cors import CORS, cross_origin
import os

ydl_opts = {
    'quiet': True
}
ytl = yt_dlp.YoutubeDL(ydl_opts)

app = Flask(__name__)
cors = CORS(app)
app.config['CORS_HEADERS'] = 'Content-Type'

conn = ps.connect(dbname='main', user='postgres',
                  password='miku', host=os.environ['DB_URL'], port='5432')
cursor = conn.cursor()


def token_check(request):
    cursor.execute("select 1 from sessions where token = %s limit 1;", (request.args["token"],))

    if len(cursor.fetchall()) == 0:
        return False
    return True


@app.route("/get_info", methods=['GET'])
def get_info():

    if "url" not in request.args:
        return "wrong arguments"

    #if not token_check(request):
    #    return "access denied", 403

    try:
        dictMeta = ytl.extract_info(request.args['url'], download=False)
    except:

        type, value, traceback = sys.exc_info()
        value = str(value).split(' ')

        stream_id = extract.video_id(request.args['url'])
        return json.dumps({
            "formats": [
                {
                    "id": "best",
                    "fps": 60,
                    "bitrate": "6000",
                    "resolution": "best"
                },
                {
                    "id": "medium",
                    "fps": 30,
                    "bitrate": "4000",
                    "resolution": "medium"
                },
                {
                    "id": "bad",
                    "fps": 30,
                    "bitrate": "2000",
                    "resolution": "bad"
                }
            ],
            "thumbnail": f"https://i.ytimg.com/vi/{stream_id}/maxresdefault.jpg",
            "is_wait": True,
            "time_to_start": value[-2] + ' ' + value[-1]
        })


    if 'duration' not in dictMeta:
        is_stream = dictMeta['extractor_key'] == 'TwitchStream' or dictMeta['is_live']
    else:
        is_stream = dictMeta['is_live']

    if not is_stream:
        return json.dumps({"error": "not a stream"}), 400

    formats = []
    for ff in reversed(dictMeta["formats"]):
        f = {
            "id": ff["format_id"],
            "fps": ff["fps"],
            "bitrate": ff["tbr"],
            "resolution": ff["resolution"]
        }
        formats.append(f)

    thumbnail = ""

    for thumbnail_ in reversed(dictMeta['thumbnails']):
        if 'resolution' in thumbnail_ and thumbnail_['resolution'] == '1280x720':
            thumbnail = thumbnail_['url']
            break

    if thumbnail == "":
        thumbnail = dictMeta['thumbnails'][0]['url']

    return json.dumps({
        "formats": formats,
        "thumbnail": thumbnail,
        "is_wait": False
    })


@app.route("/get_size", methods=['GET'])
def get_size():

    if "url" not in request.args or "time" not in request.args or "format_id" not in request.args:
        return "wrong arguments", 400

    url = request.args["url"]
    try:
        time = datetime.time.fromisoformat(request.args["time"])
    except ValueError:
        return json.dumps({"error": "wrong time"}), 400

    format_id = request.args["format_id"]

    dictMeta = ytl.extract_info(url, download=False)

    ff = list(filter(lambda x: format_id == x["format_id"], dictMeta["formats"]))[0]

    if not ff:
        return json.dumps({"error": "wrong format_id"}), 400

    size = ff["tbr"] * (time.hour * 3600 + time.minute * 60 + time.second)
    return json.dumps({"size": size})
