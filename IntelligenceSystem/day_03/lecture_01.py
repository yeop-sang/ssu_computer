# -*- coding: utf-8 -*-

# 데이터 분석(머신러닝 / 딥러닝) 수행하는 과정

# 1. 데이터의 적재 (로딩)

import pandas as pd
# - 유방암 데이터 셋 (분류용 데이터 셋, 암의 악성, 악성X 여부 판별)
from sklearn.datasets import load_breast_cancer

# 사이킷런에서 제공하는 오픈 데이터 셋
data = load_breast_cancer()
print(data.keys())

# 설명변수(X) : 특정 종속변수를 유추하기 위해서 정의된 데이터 셋
X = pd.DataFrame(data.data, columns = data.feature_names)
# 종속변수(y) : 정답 데이터, Label
y = pd.Series(data.target)


# 2. 데이터의 관찰(탐색) EDA

# 설명변수 X에 대한 데이터 탐색

# - 데이터의 개수, 컬럼의 개수
# - 각 컬럼 데이터에 대한 결측 데이터의 존재 유무
# - 각 컬럼 데이터의 데이터 타입
#   (반드시 수치형의 데이터만 머신러닝에 활용할 수 있음)
print(X.info())

# pandas 라이브러리의 옵션을 설정
# - 출력 컬럼의 개수를 제어
pd.options.display.max_columns = 30
# - 출력 컬럼의 행를 제어
# pd.options.display.max_rows = 100

# 데이터를 구성하는 각 컬럼들에 대해서 기초 통계 정보를 확인
# - 데이터의 개수
# - 평균, 표준편차
# - 최소, 최대값
# - 4분위수

# 데이터를 구성하는 컬럼들의 스케일 부분을 중점적으로 체크
# (스케일 : 특정 컬럼 값의 범위)
# - 각 컬럼 별 스케일의 오차가 존재하는 경우
#   머신러닝 알고리즘의 종류에 따라서 스케일 전처리가 필요함
print(X.describe())


# 종속변수 - y
print(y)

# 종속변수의 값이 범주형인 경우 (카테고리)
# 범주형 값의 확인 및 개수 체크
print(y.value_counts())

# 범주형 종속변수의 경우 값의 개수 비율이 중요함
# 값의 개수 비율이 중요한 이유!!!
# - 머신러닝으로 예측하려는 데이터는 데이터의 비중이 작음
# - 극단적인 케이스로 악성 1%, 악성X 99%
#   (악성이 아님으로 예측만 수행해도 99%의 정확도를 가짐)
# - 데이터의 비율에서 많은 차이가 발생하는 경우
#   (오버샘플링 / 언더샘플링)
print(y.value_counts() / len(y))


# 데이터 전처리는 학습 데이터에 대해서 수행
# 테스트 데이터는 학습 데이터에 반영된 결과를 수행

# 데이터 스케일 처리
# from sklearn.preprocessing import MinMaxScaler
# scaler = MinMaxScaler()

# 설명변수 X의 전체 데이터의 최소 / 최대값을 학습
# scaler.fit(X)
# 설명변수 X의 모든 데이터를 0 ~ 1 사이의 값으로 변환
# X = scaler.transform(X)

# 위와같은 전처리 코드의 경우 데이터의 전체 모습을 미리 확인한 후
# 데이터가 분할되는 결과를 가지기 때문에 
# 학습의 성능은 올라가지만 실전에서 결과가 좋지 않은 모델이 생성될 수 있음




# 3. 데이터의 분할
# - 학습 (70%, 80%), 테스트 (30%, 20%) (머신러닝의 케이스)
# - 학습 (70%), 검증 (10%), 테스트 (20%) (딥러닝의 케이스)
#   (딥러닝의 경우 부분 배치 학습을 수행하여 점진적으로 학습량을
#    늘력나가는 경우가 많음, 중간 점검의 의미로 검증 데이터를 활용)

from sklearn.model_selection import train_test_split

# 사용 예제
# X_train, X_test, y_train, y_test = train_test_split(X, y,
#                                       test_size=테스트데이터비율(0.2),
#                                       train_size=테스트데이터비율(0.8),
#                                       stratify=범주형테이터인경우만 y를 입력,
#                                       random_state=임의의 정수값)

X_train, X_test, y_train, y_test = train_test_split(X, y,
                                                    test_size=0.2,
                                                    stratify=y,
                                                    random_state=30)

# 분할된 데이터의 개수 확인
print(len(X_train), len(X_test))

# train_test_split 함수의 random_state 매개변수는 
# 데이터의 분할된 값이 항상 동일하도록 유지하는 역할을 함
# - 머신러닝의 알고리즘에 사용되는 하이퍼 파라메터를 테스트할 때
#   데이터는 고정하고 머신러닝의 학습 방법만을 제어해보면서
#   성능 향상의 정보를 테스트할 수 있음
print(y_train[:5])


# stratify : 데이터가 분류형 데이터 셋인 경우에만 사용
#            (y 데이터가 범주형인 경우)
#            각 범주형 값의 비율을 유지하면서 데이터를 분할하는 역할
#            (random_state 의 값에 관계없이 비율이 유지됨)

# y 전체에 대한 값 비율
# 1    0.627417
# 0    0.372583
print(y_train.value_counts() / len(y_train))
print(y_test.value_counts() / len(y_test))



# 4. 데이터 전처리
# - 스케일 처리 (MinMax, Standard, Robust)
# - 인코딩 처리 (라벨 인코딩, 원핫인코딩)
# - 차원 축소
# - 특성 공학 ...

from sklearn.preprocessing import MinMaxScaler
scaler = MinMaxScaler()

# 전처리과정의 데이터 학습은 학습데이터를 기준으로 수행
# - 학습 데이터의 최소 / 최대값을 기준으로 스케일링을 수행 준비
scaler.fit(X_train)

# - 학습 데이터 스케일 처리 수행
X_train = scaler.transform(X_train)
# - 테스트 데이터는 학습 데이터를 기준으로 변환 과정만 수행
X_test = scaler.transform(X_test)


# 5. 머신러닝 모델의 구축
from sklearn.neighbors import KNeighborsClassifier

# - 머신러닝 모델 객체 생성
# - 각 머신러닝 알고리즘에 해당하는 하이퍼 파라메터의 제어가 필수적임
# mac 에서 n_jobs는 6이 최대!
model = KNeighborsClassifier(n_neighbors=11, n_jobs=6)

# - 머신러닝 모델 객체 학습
# - fit 메소드 사용
# - 사이킷런의 모든 머신러닝 클래스는 fit 메소드의 매개변수로
#   X, y를 입력받음
#   (X는 반드시 2차원 데이터 셋 - Pandas의 DataFrame, Python의 list ...)
#   (y는 반드시 1차원 데이터 셋 - Pandas의 Series, Python의 list ...)
model.fit(X_train, y_train)

# - 학습이 완료된 머신러닝 모델 객체 평가
# - score 메소드를 사용
# - 모델객체.score(X, y)
#   (입력된 X를 사용하여 예측을 수행하고 
#    예측된 값을 입력된 y와 비교하여 평가 결과를 반환)

# 주의사항!!!
# - 머신러닝 클래스의 타입이 분류형이라면 score 메소드는 정확도를 반환
#   (정확도 : 전체 데이터에서 정답인 데이터의 비율)
# - 머신러닝 클래스의 타입이 회귀형인 경우, score 메소드는 결정계수(R2)를 반환
#   (-값 ~ 1 사이의 값을 가짐, 1이 100% 예측 일치)
score = model.score(X_train, y_train)
print(f'Train = {score}')

score = model.score(X_test, y_test)
print(f'Test = {score}')


# 학습된 머신러닝 모델을 사용하여 예측 수행
# - predict 메소드 사용
# - model.predict(X - 설명변수)
# - 예측할 데이터 X는 반드시 2차원으로 입력되어야 함

pred = model.predict(X_train[:2])
print(pred)         # [0 1]
# 441    0
# 425    1
print(y_train[:2])

pred = model.predict(X_test[-2:])
print(pred)         # [1 1]
# 324    1
# 493    1
print(y_test[-2:])


# 학습된 머신러닝 모델이 분류형인 경우
# 확률 값으로 예측할 수 있음 (일부 클래스에선 제공 X)
# - predict_proba 메소드 사용
# - model.predict_proba(X - 설명변수)
# - 예측할 데이터 X는 반드시 2차원으로 입력되어야 함
# - 예측 확률의 기준선의 개념을 도입하여 일정 확률이상으로 예측하는 경우에만
#   예측의 결과를 수용하는 방법을 사용
proba = model.predict_proba(X_test)

# [[0. 1.]
#  [0. 1.]]
print(proba)













