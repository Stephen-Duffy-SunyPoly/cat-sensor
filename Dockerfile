FROM tomcat:latest

# Copy the web app into to the web app folder 
COPY ROOT /usr/local/tomcat/webapps/ROOT/

# Expose Tomcat's default port
EXPOSE 8080

#copy the databsae driver to tomcat's library
COPY postgresql-42.7.3.jar /usr/local/tomcat/lib

# (Optional) Add any custom Tomcat configuration here
# COPY server.xml /usr/local/tomcat/conf/server.xml