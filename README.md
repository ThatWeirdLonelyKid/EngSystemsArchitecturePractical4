# OzAgriMate Platform – Assignment 4

ISYS3459 Systems Architecture and Design

**Team:** Anagha Kulkarni (S4176385), Meryem Postaci (S4165719), Prisha Kapoor (S4172431)

A partial C++ implementation of the OzAgriMate platform, using a service-based architecture. It exposes 8 REST endpoints backed by in-memory dummy data (no real database).

## Build and run
```powershell
cmake -B build
cmake --build build --config Debug
.\build\Debug\ozagrimate.exe
```
The server runs on `http://localhost:8080`.

## Endpoints
| Method | Path | Service |
|---|---|---|
| GET | /farms | Farm Management |
| POST | /farms | Farm Management |
| PUT | /farms/{id} | Farm Management |
| DELETE | /farms/{id} | Farm Management |
| GET | /sensors?farmId={farmId} | IoT Monitoring |
| POST | /sensors | IoT Monitoring |
| GET | /recommendations?farmId={farmId} | AI Recommendation |
| GET | /notifications?userId={userId} | Notification |