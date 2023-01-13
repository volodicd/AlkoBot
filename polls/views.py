from django.shortcuts import render
from django.http import HttpResponse
from django.shortcuts import render
from polls.models import Order, Drink
from arduino.connect import drink
from django.template import RequestContext


# Create your views here.
def index(request):
    # if post request comes from the subscribe button
    # then saving user email in our database
    if 'order' in request.POST:
        drink(request.POST.get ("order"))
    return render (request, 'index.html')


