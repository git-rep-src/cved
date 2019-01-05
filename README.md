# CVED

Vulnerable docker containers manager.

![alt tag](https://i.ibb.co/7QYGvMR/cved.png)

## REQUISITES

* Add user to docker group

```shell
# Create the docker group if it doesn't already exist.
sudo groupadd docker

# Add your user to the docker group.
sudo usermod -aG docker $USER

# Log out and log back in so that your group membership is re-evaluated.
```
* Start docker daemon

```shell
# systemctl 
sudo systemctl start docker

# service
sudo service docker start
```
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
Code and docker containers are welcome.
