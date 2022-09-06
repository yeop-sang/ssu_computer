# -*- coding: utf-8 -*-

# 회귀 분석
# - 머신러닝이 예측해야하는 정답의 데이터가 연속된 수치형인 경우를 의미함
# - 분류 분석의 경우 정답 데이터는 범주형[남자/여자, 탈퇴/유지 ...]
# - 선형 방정식을 활용한 머신러닝 실습

import pandas as pd
# load_  : 연습을 위한 간단한 데이터 셋
# fetch_ : 실 데이터 셋 (상대적으로 데이터의 개수가 많음)
from sklearn.datasets import fetch_california_housing

data = fetch_california_housing()

# 설명변수 : 집값 데이터를 예측하기 위한 피처 데이터를 저장하는 변수
#           (2차원 데이터)
X = pd.DataFrame(data.data, columns=data.feature_names)
# 종속변수(정답) : 설명변수를 사용하여 예측하기 위한 변수
#                 (설명변수의 행수와 동일한 크기의 1차원 배열)
y = pd.Series(data.target)


# 설명변수의 EDA
print(X.info())

# 결측 데이터의 개수를 확인하는 방법
print(X.isnull())
print(X.isnull().sum())

pd.options.display.max_columns = 100
print(X.describe(include='all'))

# X 데이터를 구성하고 있는 각 특성(피처)들의 
# 스케일 범위를 반드시 확인
# - Population 컬럼에서 스케일의 차이가 발생하는 것을 확인할 수 있음

# 스케일을 동일한 범위로 수정하기 위한 전처리 방법
# - 정규화, 일반화
# - StandardScaler, MinMaxScaler

# 각 컬럼(특성, 피처)들에 대해서 산포도, 비율 등을 시각화 과정을
# 통해서 확인 및 전처리 체크를 수행


# 종속변수의 확인
# - 연속된 수치형 데이터 임을 확인
# - 회귀 분석을 위한 데이터 셋
print(y.head())
print(y.tail())

# 회귀 분석의 경우 중복되는 경우가 흔치 않으므로
# 분류 분석과 같이 value_counts 메소드를 사용하여
# 값의 개수를 확인하는 과정은 생략함
# print(y.value_counts())

# 데이터의 분할
# - 회귀 분석을 위한 데이터 셋의 경우
#   y 데이터 내부의 값의 분포 비율을 유지할 필요가 없음
# - 비율이 중요한 경우에는 stratify를 사용하여 층화추출방법을 사용
from sklearn.model_selection import train_test_split
X_train,X_test,y_train,y_test = train_test_split(X, y,
                                                 stratify=y,
                                                 test_size=0.3,                                                 
                                                 random_state=1)

# shape 속성 : 데이터의 차원 정보를 반환
print(X_train.shape, X_test.shape)
# len 함수를 사용
print(len(y_train), len(y_test))


# 선형 방정식을 기반으로 회귀 예측을 수행할 수 있는 클래스
# - y = x1 * w1 + x2 * w2 + ..... xN * wN + b
# - LinearRegression 클래스의 학습은 X 데이터를 구성하는 
#   각 컬럼(특성, 피처) 별 최적화된 가중치와 절편의 값을
#   계산하는 과정을 수행
from sklearn.linear_model import LinearRegression

# 머신러닝 객체의 생성
model = LinearRegression(n_jobs=6)

# 학습
model.fit(X_train, y_train)

# 평가 (score 메소드)
# - 분류를 위한 클래스
#  : 정확도(Accuracy) : 전체 데이터 중 정답으로 맞춘 비율
# - 회귀를 위한 클래스
#  : R2 Score(결정계수) : - ~ 1 까지의 범위를 가지는 평가값

# R2(결정계수) 계산 공식
# 1 - ((실제 정답과 모델이 예측한 값의 차이의 제곱 값 합계) / 
#      (실제 정답과 실제 정답의 평균 값 차이의 제곱값 합계))

# R2(결정계수)의 값이 0 인 경우
# - 머신러닝 모델이 예측한 값이 전체 정답의 평균으로만 예측하는 경우
# - 머신러닝 모델의 학습이 부족함 (과소적합)

# R2(결정계수)의 값이 1 인 경우
# - 머신러닝 모델이 예측하는 값이 실제 정답과 완벽하게 일치하는 경우
# - 머신러닝 모델의 학습이 잘 진행됨을 확인 (과대적합의 의심...)

# R2(결정계수)의 값이 0 보다 작은 경우
# - 머신러닝 모델이 예측하는 값이 정답들의 평균조차 예측하지 못하는 경우
# - 머신러닝 모델의 학습이 부족함 (과소적합)

# - R2(결정계수)의 값은 0.7, 0.8 이상을 목표치로 설정

# - 학습
score = model.score(X_train, y_train)
print(f'Train : {score}')

# - 테스트
score = model.score(X_test, y_test)
print(f'Test : {score}')


# - 예측
# - 테스트 데이터의 가장 앞 데이터를 사용하여 예측을 수행
pred = model.predict(X_test.iloc[:1])
print(pred)

# 머신러닝 모델이 학습한 기울기(가중치), 절편을 확인
# - 기울기(가중치)
print(model.coef_)
# - 절편(Bias)
print(model.intercept_)

# 선형 방정식을 기반으로 회귀 예측을 수행할 수 있는 클래스
# - y = x1 * w1 + x2 * w2 + ..... xN * wN + b

pred = 3.25 * model.coef_[0] + 39.0 * model.coef_[1] + \
    4.503205 * model.coef_[2] + 1.073718 * model.coef_[3] + \
        1109.0 * model.coef_[4] + 1.777244 * model.coef_[5] + \
            34.06 * model.coef_[6] + -118.36 * model.coef_[7] + \
                model.intercept_
print(pred)


# 회귀분석을 위한 머신러닝 모델의 평가 함수
# - score 메소드를 사용 : R2(결정계수)

# - R2(결정계수) : 데이터에 관계없이 동일한 결과의 범위를 사용하여 모델을 평가

# - 평균절대오차 : 실제 정답과 모델이 예측한 값의 차이를 절대값으로 평균
#                 (머신러닝 모델이 예측한 값의 신뢰 범위)
# - 평균절대오차비율 : 실제 정답과 모델이 예측한 값의 비율 차이를 절대값으로 평균

# - 평균제곱오차 : 실제 정답과 모델이 예측한 값의 차이의 제곱값 평균
#                 (머신러닝/딥러닝 모델의 오차 값을 계산할 때 사용)

# R2(결정계수)
from sklearn.metrics import r2_score
# 평균절대오차
from sklearn.metrics import mean_absolute_error
# 평균절대오차비율
from sklearn.metrics import mean_absolute_percentage_error
# 평균제곱오차
from sklearn.metrics import mean_squared_error

# 평가함수 사용
# - 모든 평가 함수는 사용법이 동일함

# 평가를 위해서는 머신러닝 모델이 예측한 값이 필요함 (공통)
pred = model.predict(X_train)

# 평균절대오차
mae = mean_absolute_error(y_train, pred)
print(f'MAE : {mae}')

print(y_train.describe())

# 평균절대오차비율
mape = mean_absolute_percentage_error(y_train, pred)
print(f'MAPE : {mape}')

# 선형 모델이 학습한 가중치를 활용하여 중요도를 파악하는 방법
# - 설명변수 X를 구성하는 각 특성 별 중요도
# - 특정 컬럼(특성, 피처)에 대한 가중치의 값이 0 이라면 
#   결과에 영향을 주지않는 특성 임을 확인11
# - 특정 컬럼에 대한 가중치가 다른 컬럼에 비해 상대적으로 높음
#   (회귀 분석의 경우 해당 특성의 값은 종속변수의 값을 증가시키기 
#    위해서 중요도가 높음)
# - 가중치의 절대값이 클 수록 영향력이 높은 특성임!
print(X.info())
print(model.coef_)

# LinearRegression 클래스는 학습 데이터를 예측하기 위해서
# 각각의 특성 별로 최적화된 가중치의 값을 계산하는 머신러닝 알고리즘.
# - LinearRegression이 학습한 가중치는 학습 데이터에 베스트 핏이 되는 가중치

# 머신러닝을 개발하는 이유???
# - 과거의 데이터를 통해서 새로운 데이터의 결과를 예측하기 위함
# - (생성된 가중치는 학습 데이터에 핏팅 - 완벽하게 베스트 핏팅)
# - (새로운 데이터에 잘 적합할 수 있을까?????)

# 선형 모델은 위의 문제를 해결하기 위해서 제약의 방식을 사용
# - L1 제약 (Lasso)
# - L2 제약 (Ridge)






















