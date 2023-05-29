from flask import Flask, request
import json
import yt_dlp
import psycopg2 as ps
import time
import threading
import urllib
import subprocess
from threading import Thread, current_thread
import requests
import os
import datetime
import sys
from pytube import extract
import sched

STORE_SERVER = f"http://localhost:9999/"

app = Flask(__name__)

ydl_opts = {
    'quiet': True
}

ytl = yt_dlp.YoutubeDL(ydl_opts)
conn = ps.connect(dbname='main', user='postgres',
                  password='miku', host=os.environ['DB_URL'], port='5432')
cursor = conn.cursor()

recording_count = 0


def record_thread(time_len, stream_url, filename, size, service, id_):
    global recording_count
    print("test")

    # conn_local = ps.connect(dbname='main', user='postgres',
                  # password='miku', host=os.environ['DB_URL'], port='5432')
    # cursor_local = conn.cursor()
    print(time_len, stream_url, filename, size, service)
    if service == 'youtube':
        subprocess.run(
            ['ffmpeg', '-hide_banner', '-loglevel', 'error', '-err_detect', 'explode', '-y', '-i', stream_url, '-c:a',
             'copy', '-c:v', 'copy', '-t',
             time_len, f"/data/records/{filename}"],
            stdout=subprocess.PIPE)
    else:
        subprocess.run(
            ['ffmpeg', '-hide_banner', '-loglevel', 'error', '-err_detect', 'explode', '-y', '-i', stream_url, '-c:a',
             'copy', '-c:v', 'copy', '-t',
             time_len, '-f', 'mpegts', f"/data/records/{filename}"],
            stdout=subprocess.PIPE)

    print("record done!")
    print(f"update records set"
        f" status='finished',"
        f" record_file='{filename}'"
        f" where id={id_};")
    cursor.execute(
        f"update records set"
        f" status='finished',"
        f" record_file='{filename}'"
        f" where id={id_};")
    conn.commit()
    print("commit")

    recording_count -= 1


s = sched.scheduler(time.time, time.sleep)


def wait_thread(request, try_count):

    try:
        dictMeta = ytl.extract_info(request['url'], download=False)
    except:

        type, value, traceback = sys.exc_info()
        value = str(value).split(' ')
        if value[-1] == "days.":
            seconds = (int(value[-2]) - 1) * 86_400
        elif value[-1] == "hours.":
            seconds = (int(value[-2]) - 1) * 3_600
        elif value[-1] == "minutes.":
            seconds = (int(value[-2]) - 1) * 60
        elif value[-1] == "seconds.":
            seconds = int(value[-2]) + 30
        else:
            if try_count == 10:
                return
            seconds = 180
        # print("wait:" + str(seconds) + " " + request['url'])
        print(f"wait: {seconds} - {request['url']}")
        s.enter(seconds, 1, wait_thread, (request,try_count + 1))
        return

    print("start record")

    if request['format_id'] == "best":
        request['format_id'] = dictMeta["formats"][-1]["format_id"]
    elif request['format_id'] == "medium":
        request['format_id'] = dictMeta["formats"][-2]["format_id"]
    else:
        request['format_id'] = dictMeta["formats"][-3]["format_id"]

    request["is_wait"] = False
    process(request)


def process(request):
    global recording_count

    if request["is_wait"]:
        print("start wait")
        wait_thread(request, 0)
        #threading.Thread(target=wait_thread, args=(request,)).start()
        return

    dictMeta = ytl.extract_info(request['url'], download=False)

    if 'duration' not in dictMeta:
        is_stream = dictMeta['extractor_key'] == 'TwitchStream' or dictMeta['is_live']
    else:
        is_stream = dictMeta['is_live']

    if not is_stream:
        cursor.execute(f"delete from records where id='{request['id']}';")
        conn.commit()
        print("is not a stream")
        return

    stream_url = ""
    size = 0
    t = datetime.time.fromisoformat(request["time"])
    for ff in dictMeta['formats']:
        if ff["format_id"] == request['format_id']:
            stream_url = ff['url']
            size = ff["tbr"] * (t.hour * 3600 + t.minute * 60 + t.second)


    if stream_url == "":
        cursor.execute(f"delete from records where id='{request['id']}';")
        conn.commit()
        print("unknown stream url")
        return

    title = dictMeta['title']

    thumbnail = ""

    for thumbnail_ in reversed(dictMeta['thumbnails']):
        if 'resolution' in thumbnail_ and thumbnail_['resolution'] == '1280x720':
            thumbnail = thumbnail_['url']
            break

    if thumbnail == "":
        thumbnail = dictMeta['thumbnails'][0]['url']

    thumbnail_file = f"{round(time.time() * 100)}.jpg"

    if dictMeta['extractor_key'] == 'TwitchStream':
        request['service'] = 'twitch'
    elif dictMeta['extractor_key'] == 'Youtube':
        request['service'] = 'youtube'
    else:
        print("unknown service")
        return

    urllib.request.urlretrieve(thumbnail, "/data/thumbnails/" + thumbnail_file)

    # TODO дабавить хэшь
    file_name = f"{round(time.time() * 100)}.mkv"
    recording_count += 1
    # title = title.replace()

    cursor.execute(
        f"update records set"
        f" title= $${title}$$,"
        f" thumbnail= '{thumbnail_file}',"
        f" status='recording',"
        f" stream_url='{stream_url}',"
        f" size = {size}"
        f" where id='{request['id']}';")
    conn.commit()

    print("start record")

    th = threading.Thread(target=record_thread, args=(request['time'], stream_url, file_name, size, request['service'], request['id'],))

    th.daemon = True

    th.start()

    th.join()
    # cb = functools.partial(ack_message, channel, delivery_tag)
    # connection.add_callback_threadsafe(cb)


@app.route("/start_record", methods=['POST'])
def start_record():
    print("testt")
    data = request.get_json()
    process(data)
    # try:
    #     process_thread(data)
    # except:
    #     print("record error")
    return ""


@app.route("/delete", methods=['POST'])
def remove():
    print("remove")
    data = request.get_json()

    r_id = int(data["id"])

    cursor.execute(f"select record_file, thumbnail from records where id = {r_id};")

    rows = cursor.fetchall()

    if len(rows) == 0:
        return "not found", 404

    rows = rows[0]

    filename_video = rows[0]
    filename_thumbnail = rows[1]

    print(filename_video, filename_thumbnail)

    os.remove(f"/data/records/{filename_video}")
    os.remove(f"/data/thumbnails/{filename_thumbnail}")

    cursor.execute(f"delete from records where id = {r_id};")
    conn.commit()

    return ""


def heartbeat():
    while True:
        requests.get(
            f"{os.environ['BALANCER']}/queue/heart_beat?"
            f"address=http://{os.environ['HOSTNAME']}:{os.environ['FLASK_RUN_PORT']}&load={recording_count}")
        time.sleep(10)


def shed_thread():
    print("start wait thread")
    while True:
        s.run(True)
        time.sleep(1)


Thread(target=heartbeat).start()
Thread(target=shed_thread).start()
