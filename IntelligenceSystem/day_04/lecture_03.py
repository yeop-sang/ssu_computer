# -*- coding: utf-8 -*-

# 선형 모델의 성능 향상을 위한 방법
# 1. 스케일 전처리 (정규화 / 일반화)
#  - 선형 모델은 각각의 특성 대해서 가중치를 할당하는 방식
#  - 각각의 특성의 스케일이 차이가 발생하면 가중치 적용에 어려움
# 2. 차원을 확장
#  - 선형 모델의 방정식 : y = x1 * w1 + x2 * w2 ... xN * wN + b (1차 방정식)
#  - 기본적으로 데이터 분석에 1차원의 직선을 사용하여 데이터를 예측

import numpy as np

X = np.arange(1,11).reshape(-1, 1)
y = [5, 8, 10, 9, 7, 5, 3, 6, 9, 10]

import matplotlib.pyplot as plt

plt.plot(X, y, 'xb')
plt.show()

from sklearn.linear_model import LinearRegression
model = LinearRegression().fit(X, y)

# 평가
print(model.score(X, y))

pred = model.predict(X)

plt.plot(X, y, 'xb')
plt.plot(X, pred, '-g')
plt.show()


# 차원을 확장하여 데이터에 대한 성능을 극대화
# 1. 선형 방정식 (직선)
# - y = x * w + b

# 2. 다차원 선형 방정식 (포물선)
# - y = x*2 * w1 + x * w2 + b

from sklearn.preprocessing import PolynomialFeatures
# - degree 하이퍼 파라메터를 사용하여 차원을 조절
poly = PolynomialFeatures(degree=2, include_bias=False).fit(X)

X_poly = poly.transform(X)

print(X_poly)

# 차원을 2차원으로 확장한 데이터로 학습을 진행
model = LinearRegression().fit(X_poly, y)

pred = model.predict(X_poly)

plt.plot(X, y, 'xb')
plt.plot(X, pred, '-g')
plt.show()


# 데이터를 3차원의 방정식에 적합한 형태로 변환
poly = PolynomialFeatures(degree=3, include_bias=False).fit(X)

X_poly = poly.transform(X)

print(X_poly)

# 차원을 3차원으로 확장한 데이터로 학습을 진행
model = LinearRegression().fit(X_poly, y)

print(model.score(X_poly, y))

pred = model.predict(X_poly)

plt.plot(X, y, 'xb')
plt.plot(X, pred, '-g')
plt.show()



































