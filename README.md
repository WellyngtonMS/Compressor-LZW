# Compressor-LZW

Implemente um compressor e descompressor utilizando o algoritmo LZW. Considere que as mensagens são geradas por fontes com alfabeto A = {0, 1, ..., 255}. Teste o compressor/descompressor com um arquivo de texto em português de 16MB.

#### LZW
O índice do dicionário deve ser testado com diferentes tamanhos K bits (parâmetro). Exemplo: K = 9bits tamanho do dicionário: 2^9 = 512, K = 10bits tamanho do dicionário 2^10 = 1024. Teste para K = 9, 10, ..., 16 bits.

#### Observações:
- Os símbolos do arquivo de teste devem ser lidos no modo binário (números) e não no modo texto (caracteres/strings).
- O codificador deve receber como entrada um arquivo e gerar como saída o arquivo codificado.
