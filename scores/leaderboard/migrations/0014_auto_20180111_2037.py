# Generated by Django 2.0 on 2018-01-11 20:37

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('leaderboard', '0013_auto_20180108_2311'),
    ]

    operations = [
        migrations.AlterField(
            model_name='team',
            name='place',
            field=models.CharField(blank=True, default='', max_length=200),
        ),
    ]
