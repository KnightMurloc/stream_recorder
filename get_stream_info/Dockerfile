FROM python:3.11-slim
WORKDIR /app
COPY src .
RUN apt-get update && apt-get install -y postgresql-client && apt-get clean
RUN pip install -r requirements.txt

CMD [ "flask", "--app" , "main", "run", "--host=0.0.0.0"]
