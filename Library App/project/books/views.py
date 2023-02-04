from django.shortcuts import render, redirect, get_object_or_404
from django.http import HttpResponse
from .models import Books
from .forms import BooksForm
from django.contrib.auth.decorators import login_required


# Create your views here.
def index(request):
        books = Books.objects.order_by('id')
        context = {'books': books}
        return render(request, 'books/index.html', context)


@login_required(login_url='/login/')
def add(request):
        if request.method == 'POST':
                books = BooksForm(request.POST)

                if books.is_valid():
                        books = books.save(commit=False)
                        books.save()
                        return redirect('view_books')
                else:
                        context = {'form': books}
                        return render(request, 'books/add.html', context)
        else:
                books = BooksForm()
                context = {'form': books}
                return render(request, 'books/add.html', context)


def get(request, id):
        books = get_object_or_404(Books, id=id)
        context = {'books': books}
        return render(request, 'books/view.html', context)


@login_required(login_url='/login/')
def edit(request, id):
        books = get_object_or_404(Books, id=id)
        form = BooksForm(request.POST or None, instance=books)
        context = {'form': form}

        if request.method == 'POST':

                if form.is_valid():
                        books = form.save(commit=False)
                        books.save()
                        context = {'form': form}
                        return redirect('view_books')
                else:
                        context = {'form': form, 'error': 'The form was not updated successfully.'}
                        return render(request,'books/edit.html' , context)
        else:
                return render(request, 'books/edit.html', context)


@login_required(login_url='/login/')
def delete(request, id):
        books = Books.objects.get(id=id)
        try:
                books.delete()
                return redirect('view_books')
        except:
                return render(request, 'books/prohibited_delete.html')
