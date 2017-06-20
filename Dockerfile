FROM alpine:3.5

RUN mkdir /lib64 && \
    ln -s /lib/libc.musl-x86_64.so.1 /lib64/ld-linux-x86-64.so.2 && \
    mkdir -p /kp_data/config && \
    mkdir -p /opt/keyprotect/config

# TODO: Remove config copy when config is moved to remote
# this will need to include the cert files to run since this is linux and will
# look as if it is deployed
COPY config/keyprotect_db.json /opt/keyprotect/config/
COPY config/container.json /opt/keyprotect/config/production.json
EXPOSE 8985

# rename the binary
COPY db-abstraction-server /opt/keyprotect/key-management-persistence
WORKDIR /opt/keyprotect
CMD ["/opt/keyprotect/key-management-persistence"]
