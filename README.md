# EngSystemsArchitecturePractical4
Systems Architecture 

Assignment 4
CHECK LIST and description

8 method + path combinations to be made

4 http (communication methods) must be used

GET
"Show me!"
Retrieve existing farm records.
Example: GET/farms - show all farms.

POST
"Add a new one!"
Create a new farm record.
Example: POST/farms - add a farm.

PUT
"Update this one!"
Replace or update an existing farm record.
Example: PUT/farm/101 - update Farm 101.

DELETE
"Remove this one!"
Delete an existing farm record.
Example: DELETE/farms/101 - delete Farm 101

8 REST API endpoints must be made
--> rest api endpoints make http requests from our OzAgriMate application to our API using **get, post, put, delete** methods
--> think of rest application programming interface (api) as a doorway between programs that asks for something or performs an action (eg; getting farm records)

A microservice architecture structure will be used (each repository has its own database and each service owns its own data)

1st repo: Farm management service (but be more specific what do we do here?)
2nd repo: IoT monitoring service
3rd repo: AI-recommendation service
4th repo: Notification server (alerts reminders messages)
