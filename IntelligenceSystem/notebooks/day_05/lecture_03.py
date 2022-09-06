# - 선형 모델 "분류"

# **결정 트리 (분류)**
# depth를 따라 정확도가 높아지는 트리
# pure node : 한쪽으로 쏠려있는 leaf node
# pure node가 아닌 경우 다수결 결정을 하게 됨
# 결정 트리를 자주 쓰는 이유, condition 을 지정할때 단일 특성만을 고려함
#       => scale 처리(전처리의 일부)가 전혀 필요하지 않음! 있는 그대로 쓰면 된다!


# 분류형 데이터 셋
import pandas as pd
from sklearn.datasets import load_breast_cancer

data = load_breast_cancer()

# 정답변수
X = pd.DataFrame(data.data,
                 columns=data.feature_names)
# 종속변수 1차원 배열은 Series로 쓰자
y = pd.Series(data.target)

print(X.info())
X.isnull().sum()
print(X.describe(include='all'))

print(y.head())
print(y.value_counts())
print(y.value_counts() / len(y))

# 우리가 항상 봐야하는 건 scale 정보
# descision tree 만 논외

# test data 추출하기
from sklearn.model_selection import train_test_split

X_train, X_test, y_train, y_test \
    = train_test_split(
    X, y,
    test_size=0.3,
    # 가장 중요한 개념, fitting 과 testing 상황을 동일하게 유지하기 위함
    random_state=11,
    # 추가적으로 중복 안되게 비율을 맞춤, 0과 1밖에 없기때문, 추나추출기법
    stratify=y,
)

X_train.shape
X_test.shape
y_train.value_counts() / len(y_train)
y_test.value_counts() / len(y_test)

# Support vector machines
# computing power가 굉장히 길어서 scale 처리가 제일 민감함
from sklearn.svm import SVC

# **C** : 높을 수록 제약사항이 풀림
# gamma : 삼차원에 있는 데이터는 이걸로 처리함
model = SVC(
    C=0.0001,
    gamma='scale',
    class_weight='balanced',
    random_state=0
)

model.fit(X_train, y_train)

model.score(X_train, y_train)

model.score(X_test, y_test)

# 가중치 값 확인
#       - linear kernel을 사용하는 경우에만 확인가능
model.coef_

# 가장 큰 값은 항상 root node에 사용된 것!

"""
분류 모델의 평가 방법
1. 정확도
    - 전체 데이터에서 정답의 비율
    - 실제로 원하는 정답을 예측하지 못하는 오류가 있음
2. **정밀도(중요한 시험 범위)
    - 모델이 예측한 결과에서 정답인 비율
      (각 클래스 별로 분류된 값이 반환, 예) 암인 환자라고 판단한 것의 정확도와 아닌 환자라고 판단한 것의 정확도)
    - 모델이 예측한 값의 정확도
    - 정밀도가 굉장히 높은 모델은 재현율이 떨어짐
3. **재현율(중요한 시험 범위)
    - 실제 데이터 중 모델이 정답으로 예측한 비율
      (각 클래스 별로 분류된 값이 반환)

재현율과 정밀도의 적절한 비율을 맞춰주는 것이 중요함
"""

# 전체 데이터에서 얼마나 맞췄는지 표현해 주는 것
from sklearn.metrics import accuracy_score
# 정밀도를 확인하기 위한 클레스의 값이 필요
from sklearn.metrics import precision_score
# 재현율을 확인하기 위한 클래스의 값이 필요
from sklearn.metrics import recall_score

# 혼동행렬
from sklearn.metrics import confusion_matrix

pred = model.predict(X_train)
cm = confusion_matrix(y_train, y_pred=pred)

cm
"""
[    모델이 예측한    0         1
실제 정답   0       [144,       4],
실제 정답   1       [4,         246]
]
"""
# 정밀도
# 0에 대한 정밀도 : cm[0][0] / (cm[0][0] + cm[1][0])
ps = precision_score(y_train, pred, pos_label=0)

ps
# 재현율
# 0에 대한 재현율 : cm[0][0] / (cm[0][0] + cm[0][1])
rs = recall_score(y_train, pred, pos_label=0)

rs
# 정밀도가 낮고 재현율이 높은 경우, 해당 값에 대한 양이 데이터 내에 많다.

# 1에 대한 정밀도 : cm[1][1] / (cm[0][1] + cm[1][1])
ps1 = precision_score(y_train, pred, pos_label=1)
ps1

# 1에 대한 재현율 : cm[1][1] / (cm[1][0] + cm[1][1])
rs1 = recall_score(y_train, pred, pos_label=1)
rs1
