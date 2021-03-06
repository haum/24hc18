# Generated by Django 2.0 on 2018-01-08 23:04

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('leaderboard', '0011_auto_20180108_0931'),
    ]

    operations = [
        migrations.RemoveField(
            model_name='match',
            name='num_pools',
        ),
        migrations.RemoveField(
            model_name='match',
            name='scenario',
        ),
        migrations.AddField(
            model_name='matchgroup',
            name='match_generated',
            field=models.BooleanField(default=False),
        ),
        migrations.AddField(
            model_name='matchgroup',
            name='num_pools',
            field=models.IntegerField(default=1),
        ),
        migrations.AddField(
            model_name='matchgroup',
            name='number_playing_teams',
            field=models.IntegerField(default=1),
        ),
        migrations.AddField(
            model_name='matchgroup',
            name='scenario',
            field=models.CharField(default='', max_length=200),
            preserve_default=False,
        ),
        migrations.AddField(
            model_name='matchgroup',
            name='teams',
            field=models.ManyToManyField(to='leaderboard.Team'),
        ),
    ]
