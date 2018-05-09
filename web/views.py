from django.shortcuts import render
from django.http import HttpResponseRedirect, HttpResponse
from django.contrib.auth.decorators import login_required
from django.contrib.auth import authenticate, login, logout
from django.contrib import messages
from django.conf import settings
from django.views.decorators.http import require_POST

import json
import subprocess

import requests


@login_required
def index(request):
    return render(request, 'web/index.html', {})

def login_view(request):
    if request.user.is_authenticated:
        return HttpResponseRedirect('/')
    if request.method == 'POST':
        username = request.POST.get('username', '')
        password = request.POST.get('password', '')
        user = authenticate(request, username=username, password=password)
        if user is not None:
            login(request, user)
            return HttpResponseRedirect('/')
        else:
            messages.add_message(request, messages.ERROR, 'Username e password errate.')
            return HttpResponseRedirect('/login')


    return render(request, 'web/login.html', {})

def logout_view(request):
    logout(request)
    return HttpResponseRedirect('/')

def api_ping_controller(request):
    ip = settings.RELAY_CONTROLLER_IP
    status, result = subprocess.getstatusoutput("ping -c1 -w2 {}".format(ip))
    return HttpResponse(str(status == 0).lower(), content_type="application/json")
        
@require_POST
def api_action(request, relay):
    try:
        requests.post('http://{}/{}'.format(settings.RELAY_CONTROLLER_IP, relay))
        return HttpResponse(json.dumps(dict(success=True)), content_type="application/json")
    except Exception as e:
        return HttpResponse(json.dumps(dict(success=False, reason=str(e))), 
            status_code=500, content_type="application/json")
