import pandas as pd
from sklearn.datasets import fetch_california_housing

data = fetch_california_housing()

X = pd.DataFrame(data.data, columns=data.feature_names)
y = pd.Series(data.target)

pd.options.display.max_columns = 100

# 각 컬럼별 데이터의 스케일 확인
#  - 스케일의 차이가 존재함
#  - 전처리 (정규화, 일반화)
#  - MinMaxScaler, StandardScaler, ...

# 종속변수(정답) 확인
# - 분류용이 아님
# - 회귀 분석을 위한 데이터 셋
print(y.head)
print(y.tail)

# 회귀분석을 위한 데이터 셋의 경우
# y 데이터 내부의 값의 분포 비율을 유지할 필요가 없음 -> stratify=1 필요 x
# (만약 비율이 중요한 경우에는 사용할 수 있음!) -> 때에 따라서 사용할 수 있음
# 회귀 분석의 목적 : 공부를 통해서 가중치를 일정하게 맞추는 것에 목표를 둠!
from sklearn.model_selection import train_test_split

X_train, X_test, y_train, y_test = train_test_split(
    # random_state를 넣어 정확한 테스트 결과를 불어옴
    X, y, test_size=0.3, random_state=1,
    # stratify=1, 회귀 분석시에는 필요없는 옵션임
)

# 선형 방정식을 기반으로 회귀 예측을 수행할 수 있는 클라스
# - y = X1 * W1(가중치) + X2 * W2 + Xi * Wi ...  + Xn * Wn + b(임의의 보정값)
#           - 가중치, 요소를 설명할 수 있는 1차 방정식의 기울기
#           - 보정값, 요소를 설명할 수 있는 1차 방정식의 절편
#           - 가중치가 음수면 예측에 부정적인 영향을 주는 요소
#           - 극단적 가중치 수치들에 집중을 해서 분석을 한다.
# - LinearRegression의 학습은 각 컬럼 별 최적화 된 가중치와 절편의 값을 계산하는 과정을 수행한다
from sklearn.linear_model import LinearRegression

model = LinearRegression(n_jobs=-1)

# 학습
model.fit(X_train, y_train)

# 평가
# - 머신러닝 모델의 평가 결과 분석
# - 분류 모델(지난 주)    : 정확도(전체 데이터 중 맞춘 비율)
# - 회귀 모델(현재의 경우): 결정계수, 일반적으로 결정계수는 - ~ 1 사이의 값을 반환
# R2(결정계수) 계산 공식
# 1 - (실제 정답과 모델이 예측한 값의 차이의 제곱 값 합계) / (실제 정답과 정답의 평균 값 차이의 제곱값 합계)

# R2 socre의 값이 0 -> 학습된 모델은 정답의 평균을 예측하고 있다. 엄밀히 말하면 학습이 된 게 아님.
#                 1 -> 실제 정답과 모델이 예측한 값의 차이가 0, 완벽하게 예측하고 있음, 혹은 해당 상황에 과적합되었다고 판단할 수도 있음
#                <0 -> 정답의 평균조차 예측을 못하고 있음
# 따라서 R2 score는 0.8 이상을 목표로 한다.
score = model.score(X_train, y_train)

# 예측해보기
pred = model.predict(X_test.iloc[0:3])

# 1번값 계산
temp = X_test.head(1)
pred = 0
for i in range(8):
    pred += temp.iloc[0][i] * model.coef_[i]

pred += model.intercept_

# 회귀모델에서 사용하는 평가 함수
# - R2 score    : 데이터에 관계없이 동일한 결과의 범위를 사용하여 모델을 평가
# - 평균절대오차: 실제 정답과 모델이 예측한 값의 차이의 절대값 평균
#                 (머신러닝 모델이 예측한 값의 신뢰 범위)
# - 평균제곱오차: 실제 정답과 모델이 예측한 값의 차이의 제곱값 평균
#                 (머신러닝/딥러닝 모델의 오차 값을 계산할 때 사용)

# 평균절대오차
from sklearn.metrics import mean_absolute_error
# 평균절대비율
from sklearn.metrics import mean_absolute_percentage_error
# 평균제곱오차
from sklearn.metrics import mean_squared_error

# 예측값
pred = model.predict(X_train)

# 평가함수 사용
# 모든 평가함수는 사용법이 동일
# 실제 정답 그리고 예측값
mae = mean_absolute_error(y_true=y_train, y_pred=pred)
mape = mean_absolute_percentage_error(y_train, pred)

# 각 특성 별 중요도 판단(가중치를 기준으로)
# 머신러닝 개발 이유?
# 데이터를 통해서 새로운 데이터를 예측하기 위해서 하는 것
# 피팅되어 있는 가중치는 학습데이터에 fitting 되어 있는 것
# 가중치를 가지고 신규 데이터에 적합할 수 없음!

# 주요한 특성을 찾을 땐 lasso 를 이용하는 것이 그 특성을 쉽게 골라낼 수 있음...


