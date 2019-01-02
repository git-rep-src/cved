# CVED

Vulnerable docker container manager.

![alt tag](https://i.ibb.co/wRGQFk8/cved.png)

## REQUISITES

You need add your user to docker group and start docker daemon before run cved.

### ADD USER TO DOCKER GROUP

```shell
# Create the docker group if it doesn't already exist.
sudo groupadd docker

# Add your user to the docker group.
sudo usermod -aG docker $USER

# Log out and log back in so that your group membership is re-evaluated.
```
## INSTALLATION

### BUILD

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
