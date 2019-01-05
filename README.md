# CVED

Vulnerable docker containers manager.

![alt tag](https://i.ibb.co/7QYGvMR/cved.png)

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

Before run cved you need add your user to the docker group and start docker service.

```shell
# Create the docker group if it doesn't already exist.
sudo groupadd docker

# Add your user to the docker group.
sudo usermod -aG docker $USER

# Log out and log back in so that your group membership is re-evaluated.

# Start docker service.
sudo systemctl start docker
```
