FROM ubuntu:latest

RUN apt-get update && apt-get install -y git gcc g++ make wget openssl

RUN cd /home \
  && wget https://dl.bintray.com/boostorg/release/1.72.0/source/boost_1_72_0.tar.gz \
  && cd /home && tar xfz boost_1_72_0.tar.gz \
  && cd /home/boost_1_72_0 && ./bootstrap.sh \
  && cd /home/boost_1_72_0 && ./b2 install \
  && rm -rf home/boost_1_72_0.tar.gz home/boost_1_72_0

RUN mkdir /pof
COPY bin/instance /pof/instance
COPY keys/private_key.pem /pof/private_key.pem

EXPOSE 7000

CMD ["/pof/instance", "-port", "7000", "-priv", "/pof/private_key.pem"]
