FROM ubuntu:latest

RUN apt-get update && apt-get install -y git gcc g++ make wget openssl

RUN cd /home \
  && wget https://dl.bintray.com/boostorg/release/1.72.0/source/boost_1_72_0.tar.gz \
  && cd /home && tar xfz boost_1_72_0.tar.gz \
  && cd /home/boost_1_72_0 && ./bootstrap.sh \
  && cd /home/boost_1_72_0 && ./b2 install \
  && rm -rf home/boost_1_72_0.tar.gz home/boost_1_72_0

RUN mkdir /pof
COPY bin/chat_server /pof/chat_server
COPY keys/private_key.pem /pof/private_key.pem

EXPOSE 1231

CMD ["/pof/chat_server", "-port", "1231", "-priv", "/pof/private_key.pem"]
