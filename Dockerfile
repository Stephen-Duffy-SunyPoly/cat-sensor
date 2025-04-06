FROM tomcat:latest

# Set environment variables (optional)
#ENV TOMCAT_USER myuser
#ENV TOMCAT_PASSWORD mypassword

# Create a user (optional)
#RUN useradd -ms /bin/bash $TOMCAT_USER

# Set ownership of Tomcat directories to the new user (optional)
#RUN chown -R $TOMCAT_USER:$TOMCAT_USER /usr/local/tomcat

# Switch to the new user (optional)
#USER $TOMCAT_USER

# Copy the web app into to the web app folder 
COPY ROOT /usr/local/tomcat/webapps/ROOT/

# Expose Tomcat's default port
EXPOSE 8080

#copy the databsae driver to tomcat's library
COPY postgresql-42.7.3.jar /usr/local/tomcat/lib

# (Optional) Add any custom Tomcat configuration here
# COPY server.xml /usr/local/tomcat/conf/server.xml