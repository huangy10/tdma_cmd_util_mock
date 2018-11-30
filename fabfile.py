import os

from fabric.api import run, env, cd
from fabric.contrib.files import exists
from fabric.contrib.project import rsync_project


env.hosts = ["woody@vlionthu.com:9022"]


def deploy():
    remote_home = "/home/woody/tdma_cmd_util"
    local_home = os.path.dirname(__file__)
    rsync_project(local_dir=local_home, remote_dir=remote_home)