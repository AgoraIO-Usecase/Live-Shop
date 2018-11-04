package io.agora.liveshop.activities;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v7.widget.RecyclerView;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.SurfaceView;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.Toast;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import io.agora.liveshop.R;
import io.agora.liveshop.data.ChatMessage;
import io.agora.liveshop.data.Product;
import io.agora.liveshop.data.Quiz;
import io.agora.liveshop.data.QuizResult;
import io.agora.liveshop.rtc.IEventCallback;
import io.agora.liveshop.rtc.RtcEngineManager;
import io.agora.liveshop.widgets.MessageContainer;
import io.agora.liveshop.widgets.ProductDialog;
import io.agora.liveshop.widgets.QuizDialog;
import io.agora.rtc.IRtcEngineEventHandler;
import io.agora.rtc.RtcEngine;
import io.agora.rtc.video.VideoCanvas;

public class LiveRoomActivity extends BaseActivity implements View.OnClickListener {
    // For this sample, the broadcaster id is hard-coded as 1
    private static final int BROADCASTER_UID = 1;

    private static final int MSG_SEND = 1;
    private static final int MSG_FIRST_FRAME_DECODED = 2;
    private static final int MSG_PRODUCT_RECEIVED = 3;
    private static final int MSG_QUIZ_RECEIVED = 4;

    private String mChannelId;
    private ProductDialog mProductDialog;
    private QuizDialog mQuizDialog;
    private Handler mHandler;
    private MessageContainer mMsgContainer;
    private Button mBackBtn;

    private EditText mChatEditText;
    private RtcEngine mRtcEngine;
    private RtcEngineManager mRtcEngineManager;
    private FrameLayout mRemoteView;

    private boolean mBroadcasterRemoteViewInitialized = false;

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
            // This is the recommended time to initialize the remote views,
            // cause we need their ids to initialize remote views.
            // Here we initialize the broadcaster remote view once, leaving other possible
            // remote videos ignored (product video will be initialized when receiving the
            // product SEI info in the onReceiveSEI callback).
            // For simplicity, we use hard-coded id for the broadcaster.
            if (uid == BROADCASTER_UID && !mBroadcasterRemoteViewInitialized) {
                Message.obtain(mHandler, MSG_FIRST_FRAME_DECODED, uid, elapsed).sendToTarget();
                mBroadcasterRemoteViewInitialized = true;
            }
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
        setContentView(R.layout.live_room_activity_layout);
        initUI();

        mHandler = new Handler(getMainLooper()) {
            @Override
            public void handleMessage(Message msg) {
                switch (msg.what) {
                    case MSG_SEND:
                        if (!(msg.obj instanceof String)) {
                            break;
                        }

                        String message = (String) msg.obj;
                        if (message.isEmpty()) {
                            Toast.makeText(LiveRoomActivity.this,
                                    R.string.toast_text_empty_message, Toast.LENGTH_SHORT).show();
                        } else {
                            // Only display messages of the current user for now
                            mMsgContainer.addMessage(new ChatMessage("You:  ", message));
                        }
                        mChatEditText.setText("");
                        break;
                    case MSG_FIRST_FRAME_DECODED:
                        int uid = msg.arg1;

                        if (mRemoteView.getChildCount() >= 1) {
                            // Supports only one remote video
                            return;
                        }

                        SurfaceView surface = RtcEngine.CreateRendererView(getBaseContext());
                        surface.setZOrderMediaOverlay(true);
                        mRemoteView.addView(surface);
                        mRtcEngine = mRtcEngineManager.getRtcEngine();
                        mRtcEngine.setupRemoteVideo(new VideoCanvas(surface, VideoCanvas.RENDER_MODE_HIDDEN, uid));
                        break;
                    case MSG_PRODUCT_RECEIVED:
                        Product product = (Product) msg.obj;
                        if (mProductDialog != null && mProductDialog.isShowing()) {
                            mProductDialog.dismiss();
                        }

                        mProductDialog = new ProductDialog(LiveRoomActivity.this,
                                findViewById(R.id.product_dialog_layout), mRtcEngine);
                        mProductDialog.show(product);
                        break;
                    case MSG_QUIZ_RECEIVED:
                        Quiz quiz = (Quiz) msg.obj;
                        if (mQuizDialog != null && mQuizDialog.isShowing()) {
                            mQuizDialog.dismiss();
                        }

                        mQuizDialog = new QuizDialog(LiveRoomActivity.this,
                                findViewById(R.id.quiz_dialog_layout));
                        mQuizDialog.show(quiz);
                        break;
                }
            }
        };
    }

    private void initUI() {
        mChatEditText = findViewById(R.id.live_chat_edit_text);
        mChatEditText.setSelected(false);
        mChatEditText.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {
                // Do nothing
            }

            @Override
            public void onTextChanged(CharSequence s, int start, int before, int count) {
                // Intercept soft keyboard "enter" event
                if (count == 1 && before == 0 && (start + count == s.length())) {
                    // Conditions to determine when single character is input:
                    // "count" is used to see if only one character is changed;
                    // "before" is zero means that this is an input operation
                    // (rather than deletion, for example);
                    // start + count should be the length of the new String,
                    // in case that the text is changed only some character
                    // in the middle of the String.
                    char newChar = s.charAt(start);
                    if (newChar == '\n') {
                        // exclude the newline character at the end
                        String content = mChatEditText.getText().toString().substring(0, start);
                        Message.obtain(mHandler, MSG_SEND, content).sendToTarget();
                    }
                }
            }

            @Override
            public void afterTextChanged(Editable s) {
                // Do nothing
            }
        });

        mMsgContainer = new MessageContainer((RecyclerView) findViewById(R.id.live_chat_recycler_view));

        mBackBtn = findViewById(R.id.live_back_btn);
        mBackBtn.setOnClickListener(this);

        mRemoteView = findViewById(R.id.remote_video_layout);
    }

    @Override
    public void onAllPermissionsGranted() {
        mChannelId = getIntent().getStringExtra(EXTRA_KEY_CHANNEL);
        mRtcEngineManager = getRtcEngineManager();
        mRtcEngineManager.init();
        mRtcEngineManager.addEventCallback(mCallback);
        mRtcEngineManager.joinChannel(null, mChannelId, null, 0);
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

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.live_back_btn:
                finish();
                break;
        }
    }
}
