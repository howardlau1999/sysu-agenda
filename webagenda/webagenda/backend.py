from django.contrib.auth.models import User
import pyagenda

class AgendaBackend:
    def authenticate(self, request, username=None, password=None):
        success = pyagenda.login(username, password)
        if success:
            try:
                user = User.objects.get(username=username)
            except User.DoesNotExist:
                user = User(username=username)
                user.save()
            return user
        return None
        
    def get_user(self, user_id):
        try:
            return User.objects.get(pk=user_id)
        except User.DoesNotExist:
            return None
