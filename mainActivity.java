package com.example.nodecontroll;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.fragment.app.DialogFragment;

import android.app.TimePickerDialog;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;
import android.widget.TimePicker;
import android.widget.Toast;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

public class MainActivity extends AppCompatActivity implements TimePickerDialog.OnTimeSetListener {

    TextView temp_txt;
    TextView text_on1, text_off1;
    TextView text_on2, text_off2;
    TextView text_on3, text_off3;
    TextView text_on4, text_off4;
    TextView text_on5, text_off5;

    FirebaseDatabase database   = FirebaseDatabase.getInstance();
    DatabaseReference myRef     = database.getReference();

    public String child0 = "";
    public String child1 = "";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        temp_txt    = findViewById(R.id.text_temp);
        text_on1    = findViewById(R.id.text_1_on);
        text_off1   = findViewById(R.id.text_1_off);
        text_on2    = findViewById(R.id.text_2_on);
        text_off2   = findViewById(R.id.text_2_off);
        text_on3    = findViewById(R.id.text_3_on);
        text_off3   = findViewById(R.id.text_3_off);
        text_on4    = findViewById(R.id.text_4_on);
        text_off4   = findViewById(R.id.text_4_off);
        text_on5    = findViewById(R.id.text_5_on);
        text_off5   = findViewById(R.id.text_5_off);

        myRef.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                changeText(snapshot, "kamar_satu", text_on1, text_off1);
                changeText(snapshot, "kamar_dua",  text_on2, text_off2);
                changeText(snapshot, "ruang_tamu", text_on3, text_off3);
                changeText(snapshot, "dapur",      text_on4, text_off4);
                changeText(snapshot, "teras",      text_on5, text_off5);

                String temp = snapshot.child("suhu").child("val").getValue(String.class);
                temp_txt.setText(temp);
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {

            }
        });
    }

    public void changeText(DataSnapshot snap, String parr, TextView t1, TextView t2){
        Integer onH  = snap.child(parr).child("on").child("jam").getValue(Integer.class);
        Integer onM  = snap.child(parr).child("on").child("menit").getValue(Integer.class);
        Integer offH = snap.child(parr).child("off").child("jam").getValue(Integer.class);
        Integer offM = snap.child(parr).child("off").child("menit").getValue(Integer.class);

        String on;
        String off;

        if (onH <= 9){
            if (onM <= 9){
                on = "0" + onH + ":0" + onM;
            } else {
                on = "0" + onH + ":" + onM;
            }
        } else {
            if (onM <= 9){
                on = onH + ":0" + onM;
            } else {
                on = onH + ":" + onM;
            }
        }

        if (offH <= 9){
            if (offM <= 9){
                off = "0" + offH + ":0" + offM;
            } else {
                off = "0" + offH + ":" + offM;
            }
        } else {
            if (offM <= 9){
                off = offH + ":0" + offM;
            } else {
                off = offH + ":" + offM;
            }
        }

        t1.setText(on);
        t2.setText(off);
    }

    public void on1(View view) {
        DialogFragment timePickerDialog = new TimePickerFragment();
        timePickerDialog.show(getSupportFragmentManager(), "Time");
        child0 = "kamar_satu";
        child1 = "on";
    }

    public void off1(View view) {
        DialogFragment timePickerDialog = new TimePickerFragment();
        timePickerDialog.show(getSupportFragmentManager(), "Time");
        child0 = "kamar_satu";
        child1 = "off";
    }

    public void on2(View view) {
        DialogFragment timePickerDialog = new TimePickerFragment();
        timePickerDialog.show(getSupportFragmentManager(), "Time");
        child0 = "kamar_dua";
        child1 = "on";
    }

    public void off2(View view) {
        DialogFragment timePickerDialog = new TimePickerFragment();
        timePickerDialog.show(getSupportFragmentManager(), "Time");
        child0 = "kamar_dua";
        child1 = "off";
    }

    public void on3(View view) {
        DialogFragment timePickerDialog = new TimePickerFragment();
        timePickerDialog.show(getSupportFragmentManager(), "Time");
        child0 = "ruang_tamu";
        child1 = "on";
    }

    public void off3(View view) {
        DialogFragment timePickerDialog = new TimePickerFragment();
        timePickerDialog.show(getSupportFragmentManager(), "Time");
        child0 = "ruang_tamu";
        child1 = "off";
    }

    public void on4(View view) {
        DialogFragment timePickerDialog = new TimePickerFragment();
        timePickerDialog.show(getSupportFragmentManager(), "Time");
        child0 = "dapur";
        child1 = "on";
    }

    public void off4(View view) {
        DialogFragment timePickerDialog = new TimePickerFragment();
        timePickerDialog.show(getSupportFragmentManager(), "Time");
        child0 = "dapur";
        child1 = "off";
    }

    public void on5(View view) {
        DialogFragment timePickerDialog = new TimePickerFragment();
        timePickerDialog.show(getSupportFragmentManager(), "Time");
        child0 = "teras";
        child1 = "on";
    }

    public void off5(View view) {
        DialogFragment timePickerDialog = new TimePickerFragment();
        timePickerDialog.show(getSupportFragmentManager(), "Time");
        child0 = "teras";
        child1 = "off";
    }

    @Override
    public void onTimeSet(TimePicker timePicker, int h, int m) {
        Toast.makeText(MainActivity.this, "Sukses mengganti waktu", Toast.LENGTH_SHORT).show();

        myRef.child(child0).child(child1).child("jam").setValue(h);
        myRef.child(child0).child(child1).child("menit").setValue(m);
    }

    public void doorlock_ON(View view) {
        myRef.child("doorlock").child("stat").setValue(1);
    }

    public void doorlock_OFF(View view) {
        myRef.child("doorlock").child("stat").setValue(0);
    }

    public void kipas_ON(View view) {
        myRef.child("kipas").child("stat").setValue(1);
    }

    public void kipas_OFF(View view) {
        myRef.child("kipas").child("stat").setValue(0);
    }

    public void gerbang_ON(View view) {
        myRef.child("gerbang").child("stat").setValue(1);
    }

    public void gerbang_OFF(View view) {
        myRef.child("gerbang").child("stat").setValue(0);
    }
}
