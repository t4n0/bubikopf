import threading
from berserk.session import TokenSession
from berserk.clients import Bots
from pathlib import Path
GAME_RUNNING = False


class Game(threading.Thread):

    def __init__(self, client, game_id, **kwargs):
        super().__init__(**kwargs)
        self.stream = client.bots.stream_game_state(game_id)

    def run(self):
        for event in self.stream:
            if event['type'] == 'gameState':
                pass

if __name__ == "__main__":
    with open(str(Path.home()) + '/.lichess_token') as f:
        token = f.read().rstrip('\n')
    bot = Bots(TokenSession(token))

    for event in bot.stream_incoming_events():
        if event['type'] == 'challenge':
            if not GAME_RUNNING:
                bot.accept_challenge(event['challenge']['id'])
            else:
                bot.decline_challenge(event['challenge']['id'])
