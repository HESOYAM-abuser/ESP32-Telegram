from flask import Flask     
import telebot
import threading
import logging

IDs = ["0000000000", "0000000000"] # Admins ID
TOKEN = "0000000000:ABCD" #Telegram token

global text
text = "0"

bot = telebot.TeleBot(TOKEN)
app = Flask(__name__) 
log = logging.getLogger('werkzeug')
log.disabled = True

def run_bot():
    bot.infinity_polling()

def run_app():
    app.run(host='0.0.0.0', port=5000, debug=False)

bot_thread = threading.Thread(target=run_bot)
app_thread = threading.Thread(target=run_app)

@bot.message_handler(commands=['start', 'help'])
def send_welcome(message):
    for id in IDs:
        if str(message.chat.id) == id:
            bot.reply_to(message, "bot has been started.")
        else:
            pass

@bot.message_handler(func=lambda message: True)
def echo_all(message):
    global text
    for id in IDs:
        if str(message.chat.id) == id:
            text = message.text
        else:
            pass

@app.route('/')       
def hello(): 
    global text
    if text != "0":
        output = text
        text = "0"
        return output
    else:
        return text

@app.route('/post/<data>') 
def hello_name(data): 
    return 'this was ur data: ' % data

try:
    if __name__=='__main__':
        bot_thread.start()
        app_thread.start()
        print('start')
        bot_thread.join()
        app_thread.join()
        print('join')
except:
    pass

   