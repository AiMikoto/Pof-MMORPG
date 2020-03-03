FROM ubuntu:latest

RUN apt-get update && apt-get install -y git gcc g++ make wget openssl

RUN cd /home && wget https://dl.bintray.com/boostorg/release/1.72.0/source/boost_1_72_0.tar.gz \
  && tar xfz boost_1_72_0.tar.gz \
  && rm boost_1_72_0.tar.gz \
  && cd boost_1_72_0 \
  && ./bootstrap.sh --prefix=/usr/local \
  && ./b2 install \
  && cd /home \
  && rm -rf boost_1_72_0

RUN mkdir /instance
COPY build/bin/instance /instance/instance

CMD ["instance/instance", "7000"]
