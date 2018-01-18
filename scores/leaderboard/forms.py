from django import forms


class TeamRegisterForm(forms.Form):
    name = forms.CharField(label="Nom de l'équipe", max_length=100,
            widget=forms.TextInput(attrs={'class': 'form-control'}))
    place = forms.CharField(label='Emplacement', max_length=400,
            widget=forms.TextInput(attrs={'class': 'form-control'}))
    repo_url = forms.CharField(label='Dépôt Git', max_length=400,
            widget=forms.TextInput(attrs={'class': 'form-control'}))
