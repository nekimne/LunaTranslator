## Как использовать офлайн-перевод с большой моделью?


### Совместимый с ChatGPT интерфейс

Также можно использовать такие инструменты, как [llama.cpp](https://github.com/ggerganov/llama.cpp), [ollama](https://github.com/ollama/ollama), [one-api](https://github.com/songquanpeng/one-api), для развертывания модели, а затем ввести адрес и модель.

Также можно использовать платформы, такие как Kaggle, для развертывания модели в облаке, в этом случае может потребоваться SECRET_KEY, в других случаях можно игнорировать параметр SECRET_KEY.

Также можно ввести API зарегистрированной большой модели (но это не обязательно), по сравнению с зарегистрированным онлайн-переводом с совместимым с ChatGPT интерфейсом, единственное отличие заключается в том, что не будет использоваться прокси.