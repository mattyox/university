from django.shortcuts import render, redirect, get_object_or_404
from django.http import HttpResponse
from .models import Loan
from .forms import LoanForm
from django.contrib.auth.decorators import login_required
import datetime
from django.utils import dateparse


# Create your views here.
@login_required(login_url='/login/')
def index(request):
        loans = Loan.objects.order_by('id')
        context = {'loans': loans}
        return render(request, 'loan/index.html', context)


@login_required(login_url='/login/')
def lend_book(request):
        if request.method == 'POST':
                loan = LoanForm(request.POST)

                if loan.is_valid():
                        loan = loan.save(commit=False)
                        today = datetime.date.today()
                        days_to_return = datetime.timedelta(days=14)
                        loan.date_of_return = dateparse.parse_date(str(today + days_to_return))
                        loan.save()
                        return redirect('view_loans')
                else:
                        context = {'form': loan}
                        return render(request, 'loan/add.html', context)
        else:
                loan = LoanForm()
                context = {'form': loan}
                return render(request, 'loan/add.html', context)


@login_required(login_url='/login/')
def get(request, id):
        loan = get_object_or_404(Loan, id=id)
        context = {'loan': loan}
        return render(request, 'loan/view.html', context)


@login_required(login_url='/login/')
def return_book(request, id):
	loan = Loan.objects.get(id=id)
	loan.delete()
	return redirect('view_loans')


@login_required(login_url='/login/')
def who_has_the_book_too_long(request):
        overdues = Loan.objects.order_by('date_of_return')
        today = str(datetime.date.today())
        overdues = Loan.objects.filter(date_of_return__lt=today)
        context = {'loans': overdues}
        return render(request, 'loan/too_long.html', context)


@login_required(login_url='/login/')
def who_has_the_book(request, book_id):
        loan = Loan.objects.order_by('id')
        loan = Loan.objects.filter(id=book_id)
        context = {'loan': loan}
        return render(request, 'loan/who_has.html', context)
