def count_combinations(coins, target):
    dp = [0] * (target + 1)
    dp[0] = 1  # Hay una sola forma de sumar 0 (no usar ninguna moneda)

    for coin in coins:
        for i in range(coin, target + 1):
            dp[i] += dp[i - coin]
    return dp[target]

# Ejemplo de uso:
monedas = [1, 2, 3]
valor_objetivo = 4
combinaciones = count_combinations(monedas, valor_objetivo)
print(f"Combinaciones posibles para {valor_objetivo}: {combinaciones}")
