from django.shortcuts import render
from django.http import HttpResponseRedirect

from django.contrib.auth.decorators import login_required

@login_required
def index(request):
    return render(request, 'web/index.html', {})

def login(request):
    if request.user.is_authenticated:
        return HttpResponseRedirect('/')
    return render(request, 'web/login.html', {})
