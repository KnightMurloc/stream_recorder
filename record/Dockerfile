FROM python:3.11-slim

WORKDIR /app

RUN apt-get update && apt-get install -y postgresql-client ffmpeg && apt-get clean

COPY src .

RUN pip install -r requirements.txt

CMD [ "flask", "--app" , "main", "run", "--host=0.0.0.0"]

