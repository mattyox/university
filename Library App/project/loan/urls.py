from django.urls import path, include
from .views import index, lend_book, get, return_book, who_has_the_book_too_long, who_has_the_book


urlpatterns = [
    path('', index, name="view_loans"),
    path('lend/', lend_book, name="lend_book"),
    path('<int:id>/', get, name='get'),
    path('return/<int:id>/', return_book, name='return_book'),
    path('too_long/', who_has_the_book_too_long, name='who_has_the_book_too_long'),
    path('who_has/<int:book_id>/', who_has_the_book, name='who_has_the_book'),
]