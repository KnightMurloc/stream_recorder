FROM ubuntu:23.04 

RUN apt-get update && \
    apt-get install -y git gcc g++ \
    cmake libjsoncpp-dev uuid-dev openssl \
    libssl-dev zlib1g-dev postgresql-all libssl-dev && \
    apt-get clean

COPY src /build
RUN cd "/build" && git clone --depth 1 --recursive https://github.com/drogonframework/drogon

RUN cd "/build" && \
    cmake "." -DBUILD_EXAMPLES=off -DCMAKE_BUILD_TYPE=Release && \
    make -j$(nproc)
RUN cd "/build" && cp "service" "/service"

RUN rm -r "/build"
RUN apt-get purge -y git gcc g++ cmake postgresql-all llvm-15 && \
    apt-get autoremove -y && apt-get clean
RUN apt-get install -y postgresql-client

EXPOSE 8848
CMD ["/service"]
