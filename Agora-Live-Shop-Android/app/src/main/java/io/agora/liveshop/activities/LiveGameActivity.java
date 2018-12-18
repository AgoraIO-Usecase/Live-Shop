package io.agora.liveshop.activities;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.SurfaceView;
import android.widget.RelativeLayout;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import io.agora.liveshop.R;
import io.agora.liveshop.data.Product;
import io.agora.liveshop.data.Quiz;
import io.agora.liveshop.data.QuizResult;
import io.agora.liveshop.rtc.IEventCallback;
import io.agora.liveshop.rtc.RtcEngineManager;
import io.agora.liveshop.widgets.ProductDialog;
import io.agora.liveshop.widgets.QuizDialog;
import io.agora.rtc.IRtcEngineEventHandler;
import io.agora.rtc.RtcEngine;
import io.agora.rtc.video.VideoCanvas;

public class LiveGameActivity extends BaseActivity {
    // For this sample, the broadcaster id is hard-coded as 1
    private static final int BROADCASTER_UID_1 = 1;
    private static final int BROADCASTER_UID_2 = 2;
    private static final int BROADCASTER_UID_STREAM = 666;

    private static final int MSG_FIRST_FRAME_DECODED = 1;
    private static final int MSG_PRODUCT_RECEIVED = 2;
    private static final int MSG_QUIZ_RECEIVED = 3;

    private static final String DEFAULT_ROOM = "hh12345";

    private RelativeLayout mGameContainer;
    private RelativeLayout mLeftContainer;
    private RelativeLayout mRightContainer;

    private ProductDialog mProductDialog;
    private QuizDialog mQuizDialog;
    private Handler mHandler;

    private RtcEngine mRtcEngine;
    private RtcEngineManager mRtcEngineManager;

    // Not all callbacks are used, but they can be easily
    // taken the advantage of to guarantee the proper behaviors
    // performed.
    private IEventCallback mCallback = new IEventCallback() {
        @Override
        public void onJoinChannelSuccess(String channel, int uid, int elapsed) {

        }

        @Override
        public void onUserJoined(int uid, int elapsed) {

        }

        @Override
        public void onUserOffline(int uid, int reason) {

        }

        @Override
        public void onError(int err) {

        }

        @Override
        public void onLeaveChannel(IRtcEngineEventHandler.RtcStats stats) {

        }

        @Override
        public void onAudioVolumeIndication(IRtcEngineEventHandler.AudioVolumeInfo[] speakers, int totalVolume) {

        }

        @Override
        public void onFirstRemoteVideoFrame(int uid, int width, int height, int elapsed) {

        }

        @Override
        public void onFirstRemoteVideoDecoded(int uid, int width, int height, int elapsed) {
            Message.obtain(mHandler, MSG_FIRST_FRAME_DECODED, uid, elapsed).sendToTarget();
        }

        @Override
        public void onReceiveSEI(String info) {
            // SEI information, embedded in video frames,  is used by
            // the broadcaster to send instant messages to audience
            // client. The information be analyzed by RTC sdk and returned here.
            // It supports either product or quiz information for now.
            parseSEIAndShow(info);
        }

        @Override
        public byte[] onSendSEI() {
            return null;
        }
    };

    private void parseSEIAndShow(String info) {
        try {
            JSONObject object = new JSONObject(info);
            String type = object.getString("type");
            switch (type) {
                case "prod":
                    Product product = parseProduct(object);
                    Message.obtain(mHandler, MSG_PRODUCT_RECEIVED, product).sendToTarget();
                    break;
                case "quiz":
                    Quiz quiz = parseQuiz(object);
                    Message.obtain(mHandler, MSG_QUIZ_RECEIVED, quiz).sendToTarget();
                    break;
            }
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }

    Product parseProduct(JSONObject object) throws JSONException {
        JSONObject jsonProduct = object.getJSONObject("product");
        Product p = new Product();
        p.setId(jsonProduct.getString("id"));
        p.setTitle(jsonProduct.getString("title"));
        p.setSold(jsonProduct.getInt("sold"));
        p.setOriginalPrice(jsonProduct.getDouble("price"));
        p.setGroupPrice(jsonProduct.getDouble("groupDeal"));
        p.setCurrency(jsonProduct.getString("currency"));
        p.setStreamId(jsonProduct.getInt("streamId"));

        // The sample provides only one variant type
        JSONObject vObject = (JSONObject) jsonProduct.getJSONArray("variants").get(0);

        ArrayList<String> colorList = new ArrayList<>();
        JSONArray colorObjectArray = vObject.getJSONArray("values");
        int length = colorObjectArray.length();
        for (int i = 0; i < length; i++) {
            colorList.add((String) colorObjectArray.get(i));
        }

        HashMap<String, List<String>> variantsMap = new HashMap<>();
        variantsMap.put(vObject.getString("name"), colorList);
        p.setVariants(variantsMap);

        return p;
    }

    Quiz parseQuiz(JSONObject object) throws JSONException {
        JSONObject questionObj = object.getJSONObject("question");
        Quiz quiz = new Quiz();
        quiz.setId(questionObj.getInt("id"));
        quiz.setContent(questionObj.getString("content"));
        quiz.setTimeout(questionObj.getInt("timeout"));
        quiz.setTotal(questionObj.getInt("total"));
        JSONArray options = questionObj.getJSONArray("options");

        ArrayList<String> optionList = new ArrayList<>();
        for (int i = 0; i < quiz.getTotal(); i++) {
            optionList.add(options.getString(i));
        }
        quiz.setOptions(optionList);

        JSONObject resultObject = object.getJSONObject("result");
        QuizResult result = new QuizResult();
        result.setResult(resultObject.getInt("result"));
        result.setTotal(resultObject.getInt("total"));
        result.setOptionCount(questionObj.getInt("total"));

        JSONObject spread = resultObject.getJSONObject("spread");

        // Need one more element to save the number of people who
        // haven't answered this question.
        // The number is saved at the index of result.getOptionCount(),
        // and is keyed by "-1" in the JSON object.
        int[] spreadArray = new int[result.getOptionCount() + 1];
        for (int i = 0; i < result.getOptionCount(); i++) {
            spreadArray[i] = spread.getInt(i + "");
        }

        spreadArray[result.getOptionCount()] = spread.getInt("-1");
        result.setSpread(spreadArray);

        quiz.setResult(result);
        return quiz;
    }

    @Override
    public void onCreate(Bundle bundle) {
        super.onCreate(bundle);
        setContentView(R.layout.activity_live_game);
        initUI();

        mHandler = new Handler(getMainLooper()) {
            @Override
            public void handleMessage(Message msg) {
                switch (msg.what) {
                    case MSG_FIRST_FRAME_DECODED:
                        int uid = msg.arg1;
                        mRtcEngine = mRtcEngineManager.getRtcEngine();
                        if (uid == BROADCASTER_UID_1) {
                            if (mLeftContainer.getChildCount() >= 1) return;
                            SurfaceView surface = RtcEngine.CreateRendererView(getBaseContext());
                            surface.setZOrderMediaOverlay(true);
                            mLeftContainer.addView(surface);
                            mRtcEngine.setupRemoteVideo(new VideoCanvas(surface, VideoCanvas.RENDER_MODE_HIDDEN, uid));
                        } else if (uid == BROADCASTER_UID_2) {
                            if (mRightContainer.getChildCount() >= 1) return;
                            SurfaceView surface = RtcEngine.CreateRendererView(getBaseContext());
                            surface.setZOrderMediaOverlay(true);
                            mRightContainer.addView(surface);
                            mRtcEngine.setupRemoteVideo(new VideoCanvas(surface, VideoCanvas.RENDER_MODE_HIDDEN, uid));
                        } else if (uid == BROADCASTER_UID_STREAM) {
                            if (mGameContainer.getChildCount() >= 1) return;
                            SurfaceView surface = RtcEngine.CreateRendererView(getBaseContext());
                            surface.setZOrderMediaOverlay(true);
                            mGameContainer.addView(surface);
                            mRtcEngine.setupRemoteVideo(new VideoCanvas(surface, VideoCanvas.RENDER_MODE_HIDDEN, uid));
                        }
                        break;
                    case MSG_PRODUCT_RECEIVED:
                        Product product = (Product) msg.obj;
                        if (mProductDialog != null && mProductDialog.isShowing()) {
                            mProductDialog.dismiss();
                        }

                        mProductDialog = new ProductDialog(LiveGameActivity.this,
                                findViewById(R.id.product_dialog_layout), mRtcEngine);
                        mProductDialog.show(product);
                        break;
                    case MSG_QUIZ_RECEIVED:
                        Quiz quiz = (Quiz) msg.obj;
                        if (mQuizDialog != null && mQuizDialog.isShowing()) {
                            mQuizDialog.dismiss();
                        }

                        mQuizDialog = new QuizDialog(LiveGameActivity.this,
                                findViewById(R.id.quiz_dialog_layout));
                        mQuizDialog.show(quiz);
                        break;
                }
            }
        };
    }

    private void initUI() {
        mGameContainer = findViewById(R.id.game_container);
        mLeftContainer = findViewById(R.id.left_container);
        mRightContainer = findViewById(R.id.right_container);
    }

    @Override
    public void onAllPermissionsGranted() {
        mRtcEngineManager = getRtcEngineManager();
        mRtcEngineManager.init();
        mRtcEngineManager.addEventCallback(mCallback);
        mRtcEngineManager.joinChannel(null, DEFAULT_ROOM, null, 0);
    }

    @Override
    protected void onResume() {
        super.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();

        if (mProductDialog != null && mProductDialog.isShowing()) {
            mProductDialog.dismiss();
        }

        if (mQuizDialog != null && mQuizDialog.isShowing()) {
            mQuizDialog.dismiss();
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        mRtcEngineManager.removeEventCallback(mCallback);
        mRtcEngineManager.leaveChannel();
    }
}
