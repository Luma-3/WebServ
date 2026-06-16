![Frame 26](https://github.com/user-attachments/assets/f1a6acda-4cf2-4f60-827e-359c9b95d99a)

<div align="center">

# 🌐 WebServ (HTTP 1.1 Server in C++98)

</div>

<p align="center">
  <br>
	<img src="https://img.shields.io/badge/Score-125-pink?style=flat-square&logo=42" alt="Score project :125"/>
	<img alt="Static Badge" src="https://img.shields.io/badge/Outstanding-2-pink?style=flat-square&logo=42">
	<img alt="GitHub repo size" src="https://img.shields.io/github/repo-size/Luma-3/WebServ?style=flat-square&logo=github&color=pink">
</p>

A fully functional **HTTP/1.1 server written in C++98** from scratch. This project is a deep dive into network programming, non-blocking I/O multiplexing, and RFC compliance.

---

## 🛠️ Features

- **HTTP/1.1 Compliance:** Handles `GET`, `POST`, and `DELETE` methods with appropriate status codes and chunked request handling.
- **CGI Support:** Executes dynamic Python and PHP scripts with full environment variable handling.
- **Resilient & Stress-Tested:** Designed to never crash, efficiently handling network errors, abrupt disconnections, and massive traffic loads.

---

## 🏗️ Architecture & Engineering Highlights

### 🔍 State-Table LR Parser
Instead of a basic sequential reader, we engineered a custom **LR Parser based on a state table**. This allows the server to systematically validate and load complex, nested Nginx-like configuration files while ensuring robust error reporting during syntax analysis.

### 🔄 High-Performance Multiplexing (`epoll`)
The core event loop is built using **Linux `epoll`**, driving a non-blocking I/O matrix. It efficiently monitors multiple file descriptors simultaneously, handling incoming connections, read/write states, and unexpected network drops smoothly without wasting CPU cycles.

### 🛡️ Quality Assurance & GitFlow
To ensure maximum code reliability and standard compliance, we established a strict **GitFlow workflow** backed by automated checks:
* Automated compilation verification scripts.
* Static code analysis integrated via `clang-tidy`.

---

## 🚀 Getting Started

### 📥 Installation & Compilation

```bash
git clone https://github.com/Luma-3/WebServ.git && cd WebServ && make
```

### 🚀 Launch default website

Run the server with our default documentation website configuration:

```bash
./webserv conf/website.conf
```
Once launched, open your browser and navigate to `http://localhost:8080` to access the built-in documentation and configuration guide.

## 🧪 Run unit tests

We built a comprehensive test suite to ensure server reliability:

```bash
make test && run ./run_all_test.sh all -y
```

## 📚 See Documentation

If you start the webserv with the classic website, you will be taken to the index page of the webserv.
On this page you will find the documentation with all the configuration information.
