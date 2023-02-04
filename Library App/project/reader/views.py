from django.shortcuts import render, redirect, get_object_or_404
from django.http import HttpResponse
from .models import Reader
from .forms import ReaderForm
from django.contrib.auth.decorators import login_required


# Create your views here.
@login_required(login_url='/login/')
def index(request):
        readers = Reader.objects.order_by('id')
        context = {'readers': readers}
        return render(request, 'reader/index.html', context)


@login_required(login_url='/login/')
def add(request):
        if request.method == 'POST':
                reader = ReaderForm(request.POST)

                if reader.is_valid():
                        reader = reader.save(commit=False)
                        reader.save()
                        return redirect('view_readers')
                else:
                        context = {'form': reader}
                        return render(request, 'reader/add.html', context)
        else:
                reader = ReaderForm()
                context = {'form': reader}
                return render(request, 'reader/add.html', context)


@login_required(login_url='/login/')
def get(request, id):
        reader = get_object_or_404(Reader, id=id)
        context = {'reader': reader}
        return render(request, 'reader/view.html', context)


@login_required(login_url='/login/')
def edit(request, id):
        reader = get_object_or_404(Reader, id=id)
        form = ReaderForm(request.POST or None, instance=reader)
        context = {'form': form}

        if request.method == 'POST':

                if form.is_valid():
                        reader = form.save(commit=False)
                        reader.save()
                        context = {'form': form}
                        return redirect('view_readers')
                else:
                        context = {'form': form, 'error': 'The form was not updated successfully.'}
                        return render(request,'reader/edit.html' , context)
        else:
                return render(request, 'reader/edit.html', context)


@login_required(login_url='/login/')
def delete(request, id):
    reader = Reader.objects.get(id=id)
    try:
        reader.delete()
        return redirect('view_readers')
    except:
        return render(request, 'reader/prohibited_delete.html')
