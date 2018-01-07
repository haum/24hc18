from django.contrib import admin

from .models import Team, BackendAuth, BackendAction


@admin.register(Team)
class TeamAdmin(admin.ModelAdmin):

    list_display = (
        'name', 'points', 'repo_url', 'place',
        'valid_repo', 'valid_buildscript', 'valid_startscript'
    )


@admin.register(BackendAuth)
class BackendAuthAdmin(admin.ModelAdmin):
    list_display = ('name', 'token', 'active', 'note')
    list_filter = ('active', )


@admin.register(BackendAction)
class BackendActionAdmin(admin.ModelAdmin):
    pass
