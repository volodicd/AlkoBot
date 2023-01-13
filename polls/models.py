from django.db import models

# Create your models here.
class Drink(models.Model):
    title = models.CharField(max_length=50, db_index=True)
    amount = models.IntegerField(null=False, blank=False)
    pump = models.IntegerField(null=False, blank=False, default=0)
    published = models.DateTimeField (auto_now_add=True, db_index=True)
    photo = models.ImageField(upload_to='uploads/', null= True, height_field=400, width_field=400)

class Order(models.Model):
    title = models.CharField (max_length=50, db_index=True)
    drink = models.ForeignKey('Drink', null=True, verbose_name='Drink', on_delete=models.SET_NULL)


