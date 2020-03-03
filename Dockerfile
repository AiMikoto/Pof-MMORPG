FROM ubuntu:latest

RUN apt-get update && apt-get install -y git gcc g++ make wget openssl

RUN cd /home && wget https://dl.bintray.com/boostorg/release/1.72.0/source/boost_1_72_0.tar.gz
RUN cd /home && tar xfz boost_1_72_0.tar.gz
RUN cd /home/boost_1_72_0 && ./bootstrap.sh
RUN cd /home/boost_1_72_0 && ./b2 install
RUN rm -rf home/boost_1_72_0.tar.gz home/boost_1_72_0

RUN mkdir /instance
COPY build/bin/instance /instance/instance
COPY build/setup/private_key.pem /instance/private_key.pem

EXPOSE 7000

CMD ["/instance/instance", "-port", "7000", "-priv", "/instance/private_key.pem"]
