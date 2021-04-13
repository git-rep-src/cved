# CVED

A tool to manage vulnerable docker containers

![cved-image](https://i.ibb.co/RbWz4D6/cved.png)

## INSTALLATION

```shell
qt >= 5.x
docker
git
```
```shell
qmake
make
sudo make install
```
## USAGE

```shell
# Create the docker group if it doesn't already exist.
sudo groupadd docker

# Add your user to the docker group.
sudo usermod -aG docker $USER

# Log out and log back in so that your group membership is re-evaluated.

# Start the docker service.
sudo systemctl start docker

# Start cved.
cved
```
## CONTRIBUTION

Code: https://github.com/git-rep-src/cved

Docker: https://github.com/cved-sources/

Contact: cved@protonmail.com 
