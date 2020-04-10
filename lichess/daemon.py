from berserk.session import TokenSession
from berserk.clients import Bots
from pathlib import Path
import bubikopf


def ConnectToLichess():
    with open(str(Path.home()) + '/.lichess_token') as f:
        token = f.read().rstrip('\n')
    return Bots(TokenSession(token))


def SendMove(lc_api, game_id, move):
    NULL_MOVE = '0000'
    if move != NULL_MOVE:
        lc_api.make_move(game_id, move)


def IsBubikopfWhite(white_player):
    BUBIKOPF_LICHESS_NAME = 'bubik0pf'
    return white_player == BUBIKOPF_LICHESS_NAME


if __name__ == "__main__":
    lc_api = ConnectToLichess()
    bk_api = bubikopf.LichessInteractor()
    print('Waiting for challenges')

    for event in lc_api.stream_incoming_events():

        if event['type'] == 'challenge':
            print("Accepted new challenge against " +
                  event['challenge']['challenger']['name'])
            lc_api.accept_challenge(event['challenge']['id'])

        elif event['type'] == 'gameStart':
            game_id = event['game']['id']
            print('Starting game ' + game_id)

            for event in lc_api.stream_game_state(event['game']['id']):

                if event['type'] == 'gameFull':
                    play_as_white = IsBubikopfWhite(event['white']['name'])
                    move = bk_api.StartGame(play_as_white)
                elif event['type'] == 'gameState':
                    move = bk_api.RespondTo(event['moves'])
                SendMove(lc_api, game_id, move)

            print('Game ' + game_id + ' is over')
            print('Waiting for challenges')
