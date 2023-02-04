from django.db import models
from books.models import Books
from reader.models import Reader


# Create your models here.
class Loan(models.Model):
    book = models.ForeignKey(Books, on_delete=models.DO_NOTHING)
    reader = models.ForeignKey(Reader, on_delete=models.DO_NOTHING)
    date_of_return = models.DateField('date of return')