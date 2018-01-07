from django import forms


class TeamRegisterForm(forms.Form):
    name = forms.CharField(label='Name', max_length=100)
    repo_url = forms.CharField(label='Git Repo', max_length=400)
