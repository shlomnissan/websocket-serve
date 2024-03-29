FROM ubuntu:18.04

# Install dependencies
RUN apt-get update \
    && apt-get install -y \
    xz-utils \
    build-essential \
    curl \
    vim \
    openssh-server \
    cmake \
    rsync \
    gdb \
    sed \
    unzip \
    && rm -rf /var/lib/apt/lists/*

# Install LLVM 9/Clang
RUN curl -SL http://releases.llvm.org/9.0.0/clang+llvm-9.0.0-x86_64-linux-gnu-ubuntu-18.04.tar.xz | tar -xJC . \
    && mv clang+llvm-9.0.0-x86_64-linux-gnu-ubuntu-18.04 clang_9.0.0 \
    && echo 'export PATH=/clang_9.0.0/bin:$PATH' >> ~/.bashrc \
    && echo 'export LD_LIBRARY_PATH=/clang_9.0.0/lib:$LD_LIBRARY_PATH' >> ~/.bashrc

# Install Boost
RUN cd /home \
    && wget http://downloads.sourceforge.net/project/boost/boost/1.70.0/boost_1_70_0.tar.gz \
    && tar xfz boost_1_70_0.tar.gz \
    && rm boost_1_70_0.tar.gz \
    && cd boost_1_70_0 \
    && ./bootstrap.sh --with-libraries=system \
    && ./b2 install

# Configure SSH
RUN mkdir /var/run/sshd && \
    echo "PermitRootLogin yes" >> /etc/ssh/sshd_config && \
    echo "PasswordAuthentication yes" >> /etc/ssh/sshd_config

ENV NOTVISIBLE "in users profile"
RUN echo "export VISIBLE=now" >> /etc/profile

EXPOSE 22
EXPOSE 8080

CMD ["/usr/sbin/sshd", "-D"]