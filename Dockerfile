# Stage 1: Build
FROM debian:bullseye-slim AS build

# Install dependencies for building C and Python
RUN apt-get update && apt-get install -y \
    build-essential \
    portaudio19-dev \
    libasound-dev \
    python3 \
    python3-pip \
    wget \
    git \
    && apt-get clean

# Install Python libraries (pandas, numpy, matplotlib)
RUN pip3 install pandas numpy matplotlib

# Copy project files to the container
COPY . /app

# Set working directory
WORKDIR /app

# Compile the C program (transmitter)
RUN gcc -o transmitter transmitter.c -lportaudio -lm

# Stage 2: Runtime environment
FROM debian:bullseye-slim

# Install runtime dependencies
RUN apt-get update && apt-get install -y \
    libportaudio2 \
    libasound2 \
    python3 \
    python3-pip \
    && apt-get clean

# Install Python libraries
RUN pip3 install pandas numpy matplotlib

# Copy compiled C program and necessary files from the build stage
COPY --from=build /app/transmitter /app/transmitter
COPY --from=build /app/cosine_values.csv /app/cosine_values.csv
COPY --from=build /app/reader.py /app/reader.py

# Set working directory
WORKDIR /app

# Default command (can be overridden by user when running the container)
CMD ["./transmitter"]
