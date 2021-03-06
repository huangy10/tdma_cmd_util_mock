import os

from fabric.api import run, env, cd
from fabric.contrib.files import exists
from fabric.contrib.project import rsync_project


env.hosts = ['compiler@192.168.199.162']


def deploy():
    remote_home = "/home/woody/Projects/archieve/origin/openwrt/package/"
    local_home = os.path.dirname(__file__)
    exclude_dir = ["src/kernel_mock", ".vscode", ".git", "__pycache__"]
    rsync_project(local_dir=local_home, remote_dir=remote_home, exclude=exclude_dir)