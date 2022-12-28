from django.db import models

# Create your models here.
class Drink(models.Model):
    title = models.CharField(max_length=50, db_index=True)
    amount = models.IntegerField(null=False, blank=False)
    pump = models.IntegerField(null=False, blank=False, default=0)
    published = models.DateTimeField (auto_now_add=True, db_index=True)
    photo = models.ImageField(upload_to='uploads/', null= True, height_field=400, width_field=400)

class Order(models.Model):
    drink = models.ForeignKey('Drink', null=False, verbose_name='Drink')
    password = models.IntegerField(null= False, blank=False, default=4324, validators=Max)

