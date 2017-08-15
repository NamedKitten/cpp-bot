import requests
import sys
import os

WEBHOOK_URL = os.environ.get("WEBHOOK_URL", "")

args = sys.argv
args.remove("jenkins.py")

status = args[0]

colour = 0xffaaff

if status == "success":
    colour = 0x2ecc71
elif status == "unstable":
    colour = 0xffff00
elif status == "failure":
    colour = 0xe74c3c

embed = {"type": "rich", "color": colour}

author = args[1]
author_url = "https://github.com/" + author
icon_url = "https://github.com/" + author + ".png"

embed["author"] = {"name": author, "url": author_url, "icon_url": icon_url}

embed["description"] = "Build was " + status + "."

build_number = os.environ.get("BUILD_DISPLAY_NAME")
branch_name = os.environ.get("BRANCH_NAME")

embed["title"] = "[cpp-bot:{}] Build {}".format(branch_name, build_number)

embed["url"] = "https://jenkins-pandentia.qcx.io/view/all/job/NamedKitten/job/cpp-bot/job/{}/{}/console".format(branch_name, build_number)

json = {
    "username": "Jenkins",
    "avatar_url": "http://jenkins.pandentia.cf/static/745f737f/images/headshot.png",
    "embeds": [embed]
}

headers = {'Content-type': 'application/json', 'Accept': 'text/plain'}

print(json)

resp = requests.post(WEBHOOK_URL, json=json)
