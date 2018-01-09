from django.contrib import admin

from .models import \
    Team,\
    BackendAuth,\
    BackendAction,\
    Match,\
    MatchGroup,\
    Participation


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

@admin.register(Match)
class MatchAdmin(admin.ModelAdmin):
    pass


@admin.register(Participation)
class ParticipationAdmin(admin.ModelAdmin):
    pass


@admin.register(MatchGroup)
class MatchGroupAdmin(admin.ModelAdmin):

    actions = ['mark_finished', 'generate_matches']

    def mark_finished(self, request, queryset):
        for m in queryset:
            m.finish_matches()

    def generate_matches(self, request, queryset):
        for m in queryset:
            m.generate_matches()

    generate_matches.short_description = 'Generate matchees for the selected groups'
    mark_finished.short_description = 'Mark related matches as finished'

